#include "WDconfig.hpp"
#include <iostream>
#include <string>
#include <vector>
#include "server_ws.hpp"
#include "Data.hpp"
#include "Digitizer.hpp"
#include "Plotter.hpp"

using WsServer = SimpleWeb::SocketServer<SimpleWeb::WS>;

/*! \fn      void CheckKeyboardCommands(WaveDumpRun_t *WDrun)
*   \brief   check if there is a key pressed and execute the relevant command
*
*   \param   WDrun:   Pointer to the WaveDumpRun_t data structure
*   \param   WDcfg:   Pointer to the WaveDumpConfig_t data structure
*   \param   BoardInfo: structure with the board info
*/
void CheckKeyboardCommands(Digitizer& digi,Plotter& plot,std::string& command)
{
	uint8_t percent;
  if(command=="0"||command=="1"||command=="2"||command=="3"||command=="4"||command=="5"||command=="6"||command=="7"||command=="8") 
  {
    int ch=std::atoi(command.c_str());
		digi.EnableChannelForPloting(ch);
  } 
	else if(command=="g")
		{
			digi.NextGroup();
		}
    else if(command=="Quit")
		{
			digi.Quit();
		}
    else if(command=="Trigger")
		{
 				digi.Trigger();
        command="Where";
		}
		else if(command=="Continuous Trigger")
		{
			digi.ContinuousTrigger();
		}
    else if(command=="Continuous Plotting")
		{
			digi.ContinuousPloting();
		}
    else if(command=="Plot")
		{
			digi.Plot();
			plot.PlotWaveform();
			plot.Save();
      command="Where";
      
		}
    else if(command=="f")
		{
				digi.f();
		}
		else if(command=="h")
		{
			digi.h();
		}
		else if(command=="Write")
		{
			digi.Write();
		}
		else if(command=="Continuous Write")
		{
				digi.ContinuousWrite();
		}
		else if(command=="Start")
		{
			digi.Start();
		}
		else if(command=="Stop")
		{
			digi.Stop();
		}
		else if(command=="Temperature")
		{
			digi.Temperature();
		}
		else if(command=="Calibrate") digi.calibrate();
		else if(command=="D")
		{
				digi.D();
			}
}


/* ########################################################################### */
/* MAIN                                                                        */
/* ########################################################################### */
int main(int argc, char *argv[])
{
    const std::string WaveDump_Release{"3.9.0"};
    const std::string WaveDump_Release_Date{"October 2018"};
    Data dat;
    WsServer server;
    Digitizer digi(dat);
    Plotter a(dat,server);
    std::string where="Release";
    server.config.port = 9876;
    auto &echo_all = server.endpoint["^/Rack/?$"];
    echo_all.on_message = [&server,&where](std::shared_ptr<WsServer::Connection> /*connection*/, std::shared_ptr<WsServer::InMessage> in_message) 
   {
    auto out_message = in_message->string();
    std::cout<<out_message<<std::endl;
    if(out_message=="Where")
    {
        for(auto &a_connection : server.get_connections()) a_connection->send(where);
    }
    else
    {
        std::string toto=out_message;
        where=toto;
    // echo_all.get_connections() can also be used to solely receive connections on this endpoint
    for(auto &a_connection : server.get_connections())
      a_connection->send(out_message);
    }
  };

  std::thread server_thread([&server]() {
    // Start WS-server
    server.start();
  });
	
  std::string ConfigFileName{""};
  int MajorNumber;
  int nCycles= 0;
  FILE *f_ini;
   
  std::cout<<"**************************************************************"<<std::endl;
  std::cout<<"                        Wave Dump "<<WaveDump_Release<<std::endl;
  std::cout<<"**************************************************************"<<std::endl;

	/* *************************************************************************************** */
	/* Open and parse default configuration file                                                       */
	/* *************************************************************************************** */

	if (argc > 1) ConfigFileName=argv[1];
	std::cout<<"Opening Configuration File "<<ConfigFileName<<std::endl;

	f_ini = fopen(ConfigFileName.c_str(), "r");
	if (f_ini == nullptr) 
  {
		digi.Quit(ERR_CONF_FILE_NOT_FOUND);
	}
	ParseConfigFile(f_ini,&dat.WDcfg);
	fclose(f_ini);

  /* *************************************************************************************** */
  /* Open the digitizer and read the board information                                       */
  /* *************************************************************************************** */
  digi.Connect();

  digi.GetInfos();
	std::cout<<"Connected to CAEN Digitizer Model "<<dat.BoardInfo.ModelName<<std::endl;
  std::cout<<"ROC FPGA Release is "<<dat.BoardInfo.ROC_FirmwareRel<<std::endl;
  std::cout<<"AMC FPGA Release is "<<dat.BoardInfo.AMC_FirmwareRel<<std::endl;


  // Check firmware rivision (DPP firmwares cannot be used with WaveDump */
  sscanf(dat.BoardInfo.AMC_FirmwareRel, "%d", &MajorNumber);
  if (MajorNumber >= 128) 
  {
    std::cout<<"This digitizer has a DPP firmware"<<std::endl;
    digi.Quit(ERR_INVALID_BOARD_TYPE);
  }
  // Get Number of Channels, Number of bits, Number of Groups of the board */
  digi.GetMoreBoardInfo();
  //load DAC calibration data (if present in flash)
  digi.LoadDACCalibration();
  // Perform calibration (if needed).
  digi.PerformCalibration();
  // mask the channels not available for this model
  digi.MaskChannels();
  // Set plot mask
  digi.SetPlotMask();
  /* *************************************************************************************** */
  /* program the digitizer                                                                   */
  /* *************************************************************************************** */
  digi.ProgramDigitizer2();

  digi.Ugly();
  // Allocate memory for the event data and readout buffer
  digi.Allocate();

  std::cout<<"Waiting for command !"<<std::endl;
  digi.setPrevRateTime();
  /* *************************************************************************************** */
  /* Readout Loop                                                                            */
  /* *************************************************************************************** */
  while(!dat.WDrun.Quit) 
  {		
      // Check for keyboard commands (key pressed)
      CheckKeyboardCommands(digi,a,where);
      if (dat. WDrun.AcqRun == 0) continue;

      /* Send a software trigger */
      if (dat.WDrun.ContinuousTrigger) digi.SoftwareTrigger();

      /* Wait for interrupt (if enabled) */
      if (dat.WDcfg.InterruptNumEvents > 0)
			{
			  digi.Interrupt();
				continue;
			}
      /* Read data from the board */
      digi.Read();

      digi.InterruptTimeout();
  }
  server_thread.join();
  return 0;
}
