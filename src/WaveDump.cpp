#include "Data.hpp"
#include "Digitizer.hpp"
#include "FileManager.hpp"
#include "Plotter.hpp"
#include "WDconfig.hpp"
#include "IXWebSocketServer.h"
#include <iostream>
#include <string>
#include <vector>
#include "CAENVMElib.h"


/*! \fn      void CheckKeyboardCommands(WaveDumpRun_t *WDrun)
 *   \brief   check if there is a key pressed and execute the relevant command
 *
 *   \param   WDrun:   Pointer to the WaveDumpRun_t data structure
 *   \param   WDcfg:   Pointer to the WaveDumpConfig_t data structure
 *   \param   BoardInfo: structure with the board info
 */
void CheckKeyboardCommands(Digitizer &digi, Plotter &plot,
                           std::string &command) {
  uint8_t percent;
  if (command == "0" || command == "1" || command == "2" || command == "3" ||
      command == "4" || command == "5" || command == "6" || command == "7" ||
      command == "8") {
    int ch = std::atoi(command.c_str());
    digi.EnableChannelForPloting(ch);
  } else if (command == "g") {
    digi.NextGroup();
  }
  else if (command == "Continuous Trigger") {
    digi.ContinuousTrigger();
  } else if (command == "Continuous Plotting") {
    digi.ContinuousPloting();
  } else if (command == "f") {
    digi.f();
  } else if (command == "h") {
    digi.h();
  } else if (command == "Write") {
    digi.Write();
  } else if (command == "Continuous Write") {
    digi.ContinuousWrite();
  } else if (command == "Temperature") {
    digi.Temperature();
  } else if (command == "Calibrate")
    digi.calibrate();
  else if (command == "D") {
    digi.D();
  }
}

/* ###########################################################################
 */
/* MAIN */
/* ###########################################################################
 */
int main(int argc, char *argv[]) 
{
  const std::string WaveDump_Release{"3.9.0"};
  const std::string WaveDump_Release_Date{"October 2018"};
  Data dat;
  ix::WebSocketServer server(9876,"192.168.1.210");
  Plotter plot(dat, server);
  FileManager file(dat, "Toto.root", 0, 36, 0);
  file.OpenFile();
  Digitizer digi(dat);
  std::string command = "Initialize";

  server.setOnConnectionCallback(
  [&server,&command](std::shared_ptr<ix::WebSocket> webSocket,std::shared_ptr<ix::ConnectionState> connectionState)
  {
  	webSocket->setOnMessageCallback(
        [webSocket, connectionState, &server,&command](const ix::WebSocketMessagePtr msg)
        {
        	if (msg->type == ix::WebSocketMessageType::Open)
                {
                    std::cerr << "New connection" << std::endl;

                    // A connection state object is available, and has a default id
                    // You can subclass ConnectionState and pass an alternate factory
                    // to override it. It is useful if you want to store custom
                    // attributes per connection (authenticated bool flag, attributes, etc...)
                    std::cerr << "id: " << connectionState->getId() << std::endl;

                    // The uri the client did connect to.
                    std::cerr << "Uri: " << msg->openInfo.uri << std::endl;

                    std::cerr << "Headers:" << std::endl;
                    for (auto it : msg->openInfo.headers)
                    {
                        std::cerr << it.first << ": " << it.second << std::endl;
                    }
                    webSocket->send(command.c_str());
                }
                else if (msg->type == ix::WebSocketMessageType::Close)
                {
			webSocket->send("NONE");
		}
                else if (msg->type == ix::WebSocketMessageType::Message)
                {
			static std::string where="Void";
                    
                    // For an echo server, we just send back to the client whatever was received by the server
                    // All connected clients are available in an std::set. See the broadcast cpp example.
                    // Second parameter tells whether we are sending the message in binary or text mode.
                    // Here we send it in the same mode as it was received.

   		    if (msg->str == "Where") 
		    {

			std::cout<<"I TRIGGER" <<msg->str<<"   "<<where<<"  "<<command <<std::endl;
                       webSocket->send(where.c_str());
    		    } 
		    else 
		    {
    			std::cout<<msg->str<<"   "<<where<<"  "<<command <<std::endl;
     	              command = msg->str;
                      where = msg->str;
      			webSocket->send(where.c_str());
     		    }




                }
            }
        );
    }
);


// Run the server in the background. Server can be stoped by calling server.stop()
server.start();



auto res = server.listen();
if (!res.first)
{
    // Error handling
    return 1;
}










  std::cout << "**************************************************************"<< std::endl;
  std::cout << "                        Wave Dump " << WaveDump_Release<< std::endl;
  std::cout << "**************************************************************"<< std::endl;
  /* ***************************************************************************************
  */
  /* Readout Loop */
  /* ***************************************************************************************
  */

  while(command!="Quit")
  {
    if(command=="Initialize")
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
			command = "Where";
    }
		else if(command=="Connect")
		{
  		// Open the digitizer 
  		digi.Connect();
      command="Where";
    }
    else if(command=="Temperature")
    {
			digi.Temperature();
			command="Where";

    }
    else if(command=="Configure")
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
  		// Set plot mask
  		digi.SetPlotMask();
  		// Have to know the number of channels; FIX IT
      plot.Init();
  		// program the digitizer 
  		digi.ProgramDigitizer();
  		digi.Ugly();
  		// Allocate memory for the event data and readout buffer
  		digi.Allocate();
  		digi.setPrevRateTime();
      command="Where";
		}
  	if(command=="Start")
  	{
      digi.Start();
    	command="Where";
  		while (command!="Stop") 
  		{
      	if(command=="Trigger")
				{
					digi.Trigger();
          command="Where";
      	}
      	else if(command=="Plot")
     	 	{
					plot.Plot();
    	  	command = "Where";
      	}

    		/* Send a software trigger */
    		if (dat.WDrun.ContinuousTrigger) digi.SoftwareTrigger();

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
							//plot.Plot();
							file.AddEvents();
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
					//plot.Plot();
					file.AddEvents();
  			}
  		}
  	}
    else if(command=="Stop")
		{
			digi.Stop();
			command="Where";
		}
    else if(command=="Disconnect")
		{
			digi.Disconnect();
			command="Where";
		}
		else if(command=="Release")
    {		
      file.CloseFile();
      command="Where";
    }
  }
  server.stop();
  return 0;
}
