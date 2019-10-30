#include "Data.hpp"
#include "Digitizer.hpp"
#include "FileManager.hpp"
#include "Plotter.hpp"
#include "WDconfig.hpp"
#include <iostream>
#include <string>
#include <vector>
#include "WebServer.hpp"

void CheckKeyboardCommands(Digitizer &digi, Plotter &plot,
                           std::string &command) {
if (command == "D") {
    digi.D();
  }
}

int main(int argc, char *argv[]) 
{
  const std::string WaveDump_Release{"3.9.0"};
  const std::string WaveDump_Release_Date{"October 2018"};
  Data dat;
  WebServer server(9876,"192.168.1.103");
  Plotter plot(dat, server.Ref());
  FileManager file(dat, "Toto.root", 0, 36, 0);
  file.OpenFile();
  Digitizer digi(dat);
	// Run the server in the background. Server can be stoped by calling server.stop()
	server.start();
	server.listen();
  std::cout << "**************************************************************"<< std::endl;
  std::cout << "                        Wave Dump " << WaveDump_Release<< std::endl;
  std::cout << "**************************************************************"<< std::endl;

  /* Readout Loop */
  while(server.Command()!="Quit")
  {
    if(server.Command()=="Event Number")
    {
          for (auto&& client : server.Ref().getClients())
                            {
                                    client->send("{\"Event\":"+std::to_string(digi.getTotalOfEvents())+"}");
  
                            }
                            server.resetCommand();
    }
    else if(server.Command()=="Initialize")
    {	
  		// Open and parse default configuration file 
      std::string ConfigFileName{""};
  		if (argc > 1)ConfigFileName = argv[1];
  		std::cout << "Opening Configuration File " << ConfigFileName << std::endl;
      FILE *f_ini;
  		f_ini = fopen(ConfigFileName.c_str(), "r");
  		if (f_ini == nullptr) digi.Quit(ERR_CONF_FILE_NOT_FOUND);
  		ParseConfigFile(f_ini, dat.WDcfg);
  		fclose(f_ini);
			server.resetCommand();
    }
		else if(server.Command()=="Connect")
		{
  		// Open the digitizer 
  		digi.Connect();
      server.resetCommand();
    }
    else if(server.Command()=="Temperature")
    {
			digi.Temperature();
			server.resetCommand();

    }
    else if(server.Command()=="Configure")
    {
      int MajorNumber{0};
			digi.GetInfos();
  		std::cout << "Connected to CAEN Digitizer Model " << dat.BoardInfo.ModelName<< std::endl;
  		std::cout << "ROC FPGA Release is " << dat.BoardInfo.ROC_FirmwareRel<< std::endl;
  		std::cout << "AMC FPGA Release is " << dat.BoardInfo.AMC_FirmwareRel<< std::endl;		
			// Check firmware rivision (DPP firmwares cannot be used with WaveDump */
  		sscanf(dat.BoardInfo.AMC_FirmwareRel, "%d", &MajorNumber);
  		if (MajorNumber >= 128) 
			{
    		std::cout << "This digitizer has a DPP firmware" << std::endl;
    		digi.Quit(ERR_INVALID_BOARD_TYPE);
  		}
  		// Get Number of Channels, Number of bits, Number of Groups of the board */
  		digi.GetMoreBoardInfo();
  		// load DAC calibration data (if present in flash)
  		digi.LoadDACCalibration();
  		// Perform calibration (if needed).
  		digi.PerformCalibration();
  		// mask the channels not available for this model
  		digi.MaskChannels();
  		// Have to know the number of channels; FIX IT
      plot.Init();
  		// program the digitizer 
  		digi.ProgramDigitizer();
  		digi.Ugly();
  		// Allocate memory for the event data and readout buffer
  		digi.Allocate();
  		digi.setPrevRateTime();
      server.resetCommand();
		}
  	else if(server.Command()=="Start")
  	{
  		while (server.Command()!="Stop") 
  		{
        if(server.Command()=="Pause")
				{
					digi.setPaused(true);
					server.resetCommand();
				}
        else if(server.Command()=="Write")
				{
					if(digi.isPaused())
					{
						file.addEvent();
        		std::cout << "Single Event saved to output files" << std::endl;
					}
					else std::cout<<"Event are YET written to the files !!!"<<std::endl;
          server.resetCommand();
				}
        else if(server.Command()=="Start")
        {
					digi.Start();
    			server.resetCommand();
        }
      	else if(server.Command()=="Trigger")
				{
					digi.Trigger();
          server.resetCommand();
      	}
      	else if(server.Command()=="Plot")
     	 	{
					plot.Plot();
    	  	server.resetCommand();
      	}
        else if(server.Command()=="Continuous Trigger")
        { 
          digi.swapContinuousTrigger();
  				server.resetCommand();
				}
        else if(server.Command()=="Continuous Plotting")
				{
      		plot.swapContinuousPlotting();
					server.resetCommand();
				}
        digi.SoftwareTrigger();
        if(!digi.isPaused())
        {
    		/* Wait for interrupt (if enabled) */
    		if (dat.WDcfg.InterruptNumEvents > 0) 
				{
      		if(digi.Interrupt()==true)
					{
						digi.InterruptTimeout();
		 				/* Analyze data */
  					for (std::size_t i = 0; i < digi.getNumberOfEvents(); i++) 
						{
							/* Get one event from the readout buffer */
    					digi.GetEvent(i);
    					/* decode the event */
    					digi.DecodeEvent();
							plot.Upload();
							if(plot.isContinuousPlotting())plot.Plot();
							file.AddEvents();
                            digi.addOneEventProcessed();
                            uint32_t event= digi.getTotalOfEvents();
                             for (auto&& client : server.Ref().getClients())
                            {

                                    client->send("{\"Event\":"+std::to_string(event)+"}");
  
                            }
                            
  					}
					}
      		continue;
    		}
    		/* Read data from the board */
    		digi.Read();
            
				digi.InterruptTimeout();
    		/* Analyze data */
  			for (std::size_t i = 0; i < digi.getNumberOfEvents(); i++) 
				{
					/* Get one event from the readout buffer */
    			digi.GetEvent(i);
    			/* decode the event */
    			digi.DecodeEvent();
          plot.Upload();
				  if(plot.isContinuousPlotting())plot.Plot();
					file.AddEvents();
                   digi.addOneEventProcessed();
                            uint32_t event= digi.getTotalOfEvents();
                             for (auto&& client : server.Ref().getClients())
                            {

                                    client->send("{\"Event\":"+std::to_string(event)+"}");
  
                            }
  			}
			}
			}
  	}
    else if(server.Command()=="Stop")
		{
			digi.Stop();
			server.resetCommand();
		}
    else if(server.Command()=="Disconnect")
		{
			digi.Disconnect();
			server.resetCommand();
		}
    else if(server.Command()=="Release")
    {		
        std::cout<<"TOTO"<<std::endl;
      file.CloseFile();
      server.resetCommand();
    }
    else if(server.Command()=="Calibrate")
    {
			digi.calibrate();
      server.resetCommand();
    }
    else if(server.Command()=="Continuous Plotting")
		{
      plot.swapContinuousPlotting();
			server.resetCommand();
		}
  }
  server.stop();
  return 0;
}
