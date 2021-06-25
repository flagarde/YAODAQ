#pragma once
#include "Board.hpp"

namespace WIENER
{

class Mpod : public yaodaq::Board
{
public:
  enum MaximumCrateValues
  {
    MaxModuleAuxSupplies = 2,     ///< The maximum number of module auxiliary supply measurements
    MaxModuleAuxTemperatures = 4, ///< The maximum number of module temperature sensors (iseg HV only)
    MaxCommunities = 4,           ///< The maximum number of SNMP community names
    MaxFans = 6,                  ///< The maximum number of fans per crate
    MaxPsAuxSupplies = 8,         ///< The maximum number of auxiliary supplies per crate
    MaxSlotsPerCrate = 10,        ///< The maximum number of slots per crate
    MaxSensors = 12,              ///< The maximum number of sensors
    MaxChannelsPerPacket = 32,    ///< The maximum number of channels in one get/set request packet
    MaxChannelsPerSlot = 100,     ///< The maximum number of channels per slot
    MaxChannelsPerCrate = 1000    ///< The maximum number of channels per crate
  };


  Mpod(const std::string& name = "Mpod");
  bool crateIsOn();
  int  getCrateStatus();
  int  getCrateStatusJson();


  bool setVoltage(const int& channel,const double& value);

  bool crateIsOff();
  bool turnOnCrate();
  bool turnOffCrate();

  int getNumberOfChannels();
  int getNumberOfModules();
  int whichModule(const int&);

  std::string getChannelName(const int&);

  std::string getModuleDescription(const int&);

  std::string getChannelsInfos();

  bool turnOn(const int& channel);
  bool turnOff(const int& channel);
  double getVoltage(const int& channel);
  float getMeasuredCurrent(const int& channel);

  double getMeasuredVoltage(const int& channel);
};

}
