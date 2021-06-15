#include "Mpod.hpp"

namespace WIENER
{
  Mpod::Mpod(const std::string& name) : Board(name)
  {
    AddMethod("turnOnChannel", GetHandle(&Mpod::turnOnChannel, *this));
  }



  void Mpod::turnOnChannel()
  {
    yaodaq::Command command;
    command.addKey("Type","Send");
    std::string method = "outputSwitch.u"+std::to_string(100);
    command.addKey("method",method);
    command.addKey("params","1");
    getConnector()->sendCommand(command);
  }
  void Mpod::turnOffChannel()
  {
    yaodaq::Command command;
    command.addKey("Type","Send");
    std::string method = "outputSwitch.u"+std::to_string(100);
    command.addKey("method",method);
    command.addKey("params","0");
    getConnector()->sendCommand(command);
  }

};
