#include "Mpod.hpp"

#include <iostream>
#include <vector>
#include "Utilities.hpp"
#include <iomanip>

namespace WIENER
{
  Mpod::Mpod(const std::string& name) : Board(name)
  {
    //Crate
    AddMethod("turnOnCrate", GetHandle( &Mpod::turnOnCrate,*this),{});
    AddMethod("turnOffCrate", GetHandle( &Mpod::turnOffCrate,*this),{});
    AddMethod("crateIsOn", GetHandle( &Mpod::crateIsOn,*this),{});
    AddMethod("crateIsOff", GetHandle( &Mpod::crateIsOff,*this),{});
    AddMethod("getCrateStatus", GetHandle( &Mpod::getCrateStatus,*this),{});




    AddMethod("turnOn", GetHandle( &Mpod::turnOn,*this),{"channel"});
    AddMethod("turnOff", GetHandle( &Mpod::turnOff,*this),{"channel"});
    AddMethod("getVoltage", GetHandle( &Mpod::getVoltage,*this),{"channel"});
    AddMethod("setVoltage", GetHandle( &Mpod::setVoltage,*this),{"channel","value"});
    AddMethod("getMeasuredVoltage", GetHandle( &Mpod::getMeasuredVoltage,*this),{"channel"});
    AddMethod("getMeasuredCurrent", GetHandle( &Mpod::getMeasuredCurrent,*this),{"channel"});
    AddMethod("getNumberOfChannels", GetHandle( &Mpod::getNumberOfChannels,*this),{});
    AddMethod("getNumberOfModules", GetHandle( &Mpod::getNumberOfModules,*this),{});
    AddMethod("whichModule", GetHandle( &Mpod::whichModule,*this),{"channel"});
    AddMethod("channelName", GetHandle( &Mpod::getChannelName,*this),{"channel"});
    AddMethod("getModuleDescription", GetHandle( &Mpod::getModuleDescription,*this),{"module"});
    AddMethod("getChannelsInfos", GetHandle( &Mpod::getChannelsInfos,*this),{});
  }

std::string Mpod::getModuleDescription(const int& module)
{
  yaodaq::Command command;
  command.addKey("Type","Receive");
  command.addKey("method",fmt::format("moduleDescription.ma{}",module));
  return getConnector()->sendCommand(command).getKey("return").asString();
}


bool Mpod::setVoltage(const int& channel,const double& value)
{
  yaodaq::Command command;
  command.addKey("Type","Send");
  command.addKey("method",fmt::format("outputVoltage.u{}",channel));
  command.addKey("params",std::to_string(value));
  return getConnector()->sendCommand(command).getKey("return").asBool();
}


std::string Mpod::getChannelsInfos()
{
  nlohmann::json infos;
  infos["modules"] = nlohmann::json::array();
  //loop over the max number of modules in the rack and find the one responding
  for(std::size_t slot=0; slot!=MaximumCrateValues::MaxSlotsPerCrate; ++slot)
  {
    std::string    slot_description = getModuleDescription(slot);
    if(!slot_description.empty())
    {
      nlohmann::json module = nlohmann::json::object();
      module["channels"] = nlohmann::json::array();
      //std::cout << "*" << slot_description << "*" << std::endl;
      std::vector<std::string> tokens;
      tokenize(slot_description, tokens, ", ");

      for(int ch = 0; ch != std::stoi(tokens[2]); ++ch)
      {
        //std::cout << "*" << ch << "*" << std::endl;
        nlohmann::json channel = nlohmann::json::object();
        int            internal_channel_number = static_cast<int>(100 * slot + ch);

        channel["slot"]           = slot;
        channel["channel"]        = ch;
        channel["channel_number"] = internal_channel_number;
        channel["name"]           = getChannelName(internal_channel_number);
        channel["current"]        = getMeasuredCurrent(internal_channel_number);
        channel["set_current"]    = getMeasuredCurrent(internal_channel_number);
        channel["voltage"]        = getMeasuredVoltage(internal_channel_number);
        channel["set_voltage"]    = getVoltage(internal_channel_number);
        module["channels"].push_back(channel);

      }
      infos["modules"].push_back(module);
    }
  }
  return infos.dump();
}


int Mpod::getNumberOfChannels()
{
  yaodaq::Command command;
  command.addKey("Type","Receive");
  command.addKey("method","outputNumber.0");
  return getConnector()->sendCommand(command).getKey("return").asInt();
}
int Mpod::getNumberOfModules()
{
  yaodaq::Command command;
  command.addKey("Type","Receive");
  command.addKey("method","moduleNumber.0");
  return getConnector()->sendCommand(command).getKey("return").asInt();
}

float Mpod::getMeasuredCurrent(const int& channel)
{
  yaodaq::Command command;
  command.addKey("Type","Receive");
  command.addKey("method",fmt::format("outputMeasurementCurrent.u{}",channel));
  return getConnector()->sendCommand(command).getKey("return").asDouble();
}

std::string Mpod::getChannelName(const int& channel)
{
  yaodaq::Command command;
  command.addKey("Type","Receive");
  command.addKey("method",fmt::format("outputName.u{}",channel));
  return getConnector()->sendCommand(command).getKey("return").asString();
}

int Mpod::whichModule(const int& channel)
{
  yaodaq::Command command;
  command.addKey("Type","Receive");
  command.addKey("method",fmt::format("outputGroup.u{}",channel));
  return getConnector()->sendCommand(command).getKey("return").asDouble();
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

int Mpod::getCrateStatusJson()
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
