#include "Mpod.hpp"

namespace WIENER
{
  Mpod::Mpod(const std::string& name) : Board(name)
  {

  }



  void Mpod::turnOnChannel()
  {
    std::cout<<"LLLLLLLLLLLLLLLLLL12121212"<<std::endl;
    yaodaq::Command command;
    command.addKey("Type","Send");
    std::string method = "outputSwitch.u"+std::to_string(100);
    command.addKey("method",method);
    command.addKey("params","1");
    getConnector()->sendCommand(command);
  }
  void Mpod::turnOffChannel()
  {
    std::cout<<"LLLLLLLLLLLLLLLLLL12121212"<<std::endl;
    yaodaq::Command command;
    command.addKey("Type","Send");
    std::string method = "outputSwitch.u"+std::to_string(100);
    command.addKey("method",method);
    command.addKey("params","0");
    getConnector()->sendCommand(command);
  }

};
