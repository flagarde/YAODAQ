 #include "DAQFile.hpp"
 #include "FileWritter.hpp"
 #include "CLI.hpp"
 #include "Interrupt.hpp"
 #include "ProgramInfos.hpp"
 
 int main(int argc, char** argv)
 {
   ProgramInfos infos;
   infos.Logo();
   Interrupt interrupt;
   CLI::App  app{"DAQFileWritter"};
   int       port{8282};
   app.add_option("-p,--port", port, "Port to listen")->check(CLI::Range(0, 65535));
   std::string host{"127.0.0.1"};
   app.add_option("-i,--ip", host, "IP of the server")->check(CLI::ValidIPV4);
   std::string filewritterName = "FileWritter";
   app.add_option("-n,--name", filewritterName, "Name of the mode")
   ->check(
     [](const std::string& t) {
       if(t == "") return "Name is empty";
           else
             return "";
     },
     "Not Empty", "Test is name is empty");
   try
   {
     app.parse(argc, argv);
   }
   catch(const CLI::ParseError& e)
   {
     spdlog::error("{}", e.what());
     return e.get_exit_code();
   }
   
   GeneralParameters::setURL("ws://" + host + ":" + std::to_string(port) + "/");
   
   Board::setConfigFile("../confs/Configs.toml");
   
   spdlog::info("Listening on IP {0} Port {1}", host, port);
   FileWritter digitizer(filewritterName);
   digitizer.setFile(std::make_unique<DAQFile>("File${ID}.root","RECREATE","File${ID}",9));
   return interrupt.wait();
 }
