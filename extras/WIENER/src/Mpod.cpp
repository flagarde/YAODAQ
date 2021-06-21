#include "Mpod.hpp"

namespace WIENER
{
  Mpod::Mpod(const std::string& name) : Board(name)
  {
    AddMethod("crateIsOn", GetHandle( &Mpod::crateIsOn,*this),{});
    AddMethod("crateIsOff", GetHandle( &Mpod::crateIsOff,*this),{});
    AddMethod("getCrateStatus", GetHandle( &Mpod::getCrateStatus,*this),{});

    AddMethod("turnOnCrate", GetHandle( &Mpod::turnOnCrate,*this),{});

    AddMethod("turnOffCrate", GetHandle( &Mpod::turnOffCrate,*this),{});
    AddMethod("turnOn", GetHandle( &Mpod::turnOn,*this),{"channel"});
    AddMethod("turnOff", GetHandle( &Mpod::turnOff,*this),{"channel"});
    AddMethod("getVoltage", GetHandle( &Mpod::getVoltage,*this),{"channel"});
    AddMethod("getMeasuredVoltage", GetHandle( &Mpod::getMeasuredVoltage,*this),{"channel"});
  }

  bool Mpod::crateIsOn()
  {
    yaodaq::Command command;
    command.addKey("Type","Receive");
    command.addKey("method","sysMainSwitch.0");
    return getConnector()->sendCommand(command).getKey("return").asBool();
  }

  bool Mpod::crateIsOff()
  {
    return !crateIsOn();
  }

int  Mpod::getCrateStatus()
{
  yaodaq::Command command;
  command.addKey("Type","Receive");
  command.addKey("method","sysStatus.0");
  return getConnector()->sendCommand(command).getKey("return").asInt();
}

int  Mpod::getCrateStatusJson()
{

}

  bool Mpod::turnOnCrate()
  {
    yaodaq::Command command;
    command.addKey("Type","Send");
    command.addKey("method","sysMainSwitch.0");
    command.addKey("params","1");
    return getConnector()->sendCommand(command).getKey("return").asBool();
  }

  bool Mpod::turnOffCrate()
  {
    yaodaq::Command command;
    command.addKey("Type","Send");
    command.addKey("method","sysMainSwitch.0");
    command.addKey("params","0");
    return getConnector()->sendCommand(command).getKey("return").asBool();
  }


  double Mpod::getVoltage(const int& channel)
  {
    yaodaq::Command command;
    command.addKey("Type","Receive");
    command.addKey("method",fmt::format("outputVoltage.u{}",channel));
    return getConnector()->sendCommand(command).getKey("return").asDouble();
  }

  double Mpod::getMeasuredVoltage(const int& channel)
  {
    yaodaq::Command command;
    command.addKey("Type","Receive");
    command.addKey("method",fmt::format("outputMeasurementTerminalVoltage.u{}",channel));
    return getConnector()->sendCommand(command).getKey("return").asDouble();
  }

  bool Mpod::turnOn(const int& channel)
  {
    yaodaq::Command command;
    command.addKey("Type","Send");
    command.addKey("method",fmt::format("outputSwitch.u{}",channel));
    command.addKey("params","1");
    getConnector()->sendCommand(command);
    return true;
  }
  bool Mpod::turnOff(const int& channel)
  {
    yaodaq::Command command;
    command.addKey("Type","Send");
    command.addKey("method",fmt::format("outputSwitch.u{}",channel));
    command.addKey("params","0");
    getConnector()->sendCommand(command);
    return true;
  }

}
