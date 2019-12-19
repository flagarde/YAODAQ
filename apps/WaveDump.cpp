#include "Data.hpp"
#include "Digitizer.hpp"
#include "FileManager.hpp"
#include "Plotter.hpp"
#include "WDconfig.hpp"
#include <iostream>
#include <string>
#include <vector>
#include "WebServer.hpp"
#include "ElogManager.h"
#include <ctime>
#include <iomanip>
#include <thread>
using namespace elogpp;

int main(int argc, char *argv[]) 
{
  if(argc!=3)
  {
	std::cout<<" config file and ip "<<std::endl;
  }
  ElogManager manager;
  const std::string WaveDump_Release{"3.9.0"};
  const std::string WaveDump_Release_Date{"October 2018"};
  Data dat;
  FileManager file(dat);
  WebServer server(2345,std::string(argv[2]));
  Plotter plot(dat, server);
  Digitizer digi(dat);
	// Run the server in the background. Server can be stoped by calling server.stop()
  std::string RunNumber{""};
	
  std::cout << "**************************************************************"<< std::endl;
  std::cout << "                        Wave Dump " << WaveDump_Release<< std::endl;
  std::cout << "**************************************************************"<< std::endl;
  server.start();
  std::thread f(&WebServer::listen,&server);
  f.detach();
  /* Readout Loop */
  while(server.Command()!="Quit")
  {
   
    ////////////////////////////////////////////////////////////////////////////////////////
    /////////////////// The Command corresponding to Status ////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////
    if(server.Command()=="INITIALIZE")
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
        server.SendStatus("INITIALIZED");
    }
    
    else if(server.Command()=="CONNECT")
    {
  		digi.Connect();
        server.SendStatus("CONNECTED");
    }
    
    else if(server.Command()=="CONFIGURE")
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
        server.SendStatus("CONFIGURED");
    }
    
    else if(server.Command()=="PAUSE")
    {
        digi.setPaused(true);
        server.SendStatus("PAUSED");
    }
    
    else if(server.Command()=="STOP")
    {
        digi.Stop();
        ElogEntry entry= manager.CreateEntry();
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::chrono::nanoseconds now2 = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch());
        long second= now2.count()/1000000000;
        entry.SetAttribute("End",std::to_string(second));
        entry.User("DAQ").To("NAS","Runs").Edit(RunNumber).Send();
        file.CloseFile();
        server.SendStatus("STOPED");
    }
    
    else if(server.Command()=="START"||server.getStatus()=="STARTED")
  	{
        
        if(server.Command()=="START"&&server.isStartedd()==false)
        {
            ElogEntry entry= manager.CreateEntry();
            ElogEntry entry2= manager.CreateEntry();
            std::cout<<"Here"<<std::endl;
            entry2.User("DAQ").To("NAS","Runs").ReceiveEntry("last");
            int ID=std::stoi(entry2.GetAttribute("ID"));
            ++ID;
            RunNumber=std::to_string(ID);
            std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
            std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
            std::chrono::nanoseconds now2 = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch());
            long second= now2.count()/1000000000;
            std::string fname = "Run_"+std::to_string(ID)+".root";
            entry.SetAttribute("Begin",std::to_string(second));
            entry.SetAttribute("Run Number",RunNumber);
            entry.SetMessage("Please say something !");
            std::cout<<"ID"<<ID<<"  "<<RunNumber<<std::endl;
            //file.Init(server.getInfos("FileName"), 0, 36, 0);
            file.Init(fname, 0, 36, 0);
            file.OpenFile();
            entry.User("DAQ").To("NAS","Runs").Send("V");
            
        }
        digi.Start();
        server.SendStatus("STARTED");
        if(server.Command()=="Write")
        {
            if(digi.isPaused())
            {
                file.addEvent();
                std::cout << "Single Event saved to output files" << std::endl;
            }
            else std::cout<<"Event are YET written to the files !!!"<<std::endl;
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
    		if (dat.WDcfg.InterruptNumEvents > 0) 
            {
                if(digi.Interrupt()==true)
                {
                    digi.InterruptTimeout();
                    if(digi.getTimeOutInfos().hasChanged())
                    {
                        if(digi.getTimeOutInfos().getDataRate()==0) std::cout << "No data..." << std::endl;
                        else
                        {
                            server.SendInfos("DataRate",std::to_string(digi.getTimeOutInfos().getDataRate()));
                            server.SendInfos("TriggerRate",std::to_string(digi.getTimeOutInfos().getTriggerRate()));
                            printf("Reading at %.2f MB/s (Trg Rate: %.2f Hz)\n",digi.getTimeOutInfos().getDataRate(),digi.getTimeOutInfos().getTriggerRate());
                        }
                    } 
                    for (std::size_t i = 0; i < digi.getNumberOfEvents(); i++) 
                    {
                        digi.GetEvent(i);
                        digi.DecodeEvent();
                        plot.Upload();
                        if(plot.isContinuousPlotting())plot.Plot();
                        file.AddEvents();
                        digi.addOneEventProcessed();
                        server.SendInfos("EventNbr",std::to_string(digi.getTotalOfEvents()));
                    }
                }
                continue;
            }
            digi.Read();
            digi.InterruptTimeout();
            if(digi.getTimeOutInfos().hasChanged())
            {
                if(digi.getTimeOutInfos().getDataRate()==0) std::cout << "No data..." << std::endl;
                else
                {
                    server.SendInfos("DataRate",std::to_string(digi.getTimeOutInfos().getDataRate()));
                    server.SendInfos("TriggerRate",std::to_string(digi.getTimeOutInfos().getTriggerRate()));
                    printf("Reading at %.2f MB/s (Trg Rate: %.2f Hz)\n",digi.getTimeOutInfos().getDataRate(),digi.getTimeOutInfos().getTriggerRate());
                }
            } 
                     
            for (std::size_t i = 0; i < digi.getNumberOfEvents(); i++) 
            {
                digi.GetEvent(i);
                digi.DecodeEvent();
                plot.Upload();
                if(plot.isContinuousPlotting())plot.Plot();
                file.AddEvents();
                digi.addOneEventProcessed();
                server.SendInfos("EventNbr",std::to_string(digi.getTotalOfEvents()));
            }
        }
    }
    
    else if(server.Command()=="CLEAR")
    {
        server.SendStatus("CLEARED");
    }
            
    else if(server.Command()=="DISCONNECT")
    {
        digi.Disconnect();
        server.SendStatus("DISCONNECTED");
    }
            
    else if(server.Command()=="RELEASE")
    {		
      server.SendStatus("RELEASED");
    }
            
            
    else if(server.Command()=="D")
    {
         digi.D();
         server.SendStatus("DED");
    }
            
            
            
            
            
            
            
            
            
    
    
     else if(server.Command()=="Event Number")
    {
        server.SendInfos("Event Number",std::to_string(digi.getTotalOfEvents()));
    }
    
    
    
    
    
    
    
    
    else if(server.Command()=="Temperature")
    {
			digi.Temperature();
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
