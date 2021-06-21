#pragma once
#include "Board.hpp"

namespace WIENER
{

class Mpod : public yaodaq::Board
{
public:
  Mpod(const std::string& name = "Mpod");
  bool crateIsOn();
  int  getCrateStatus();
  int  getCrateStatusJson();


  bool crateIsOff();
  bool turnOnCrate();
  bool turnOffCrate();
  bool turnOn(const int& channel);
  bool turnOff(const int& channel);
  double getVoltage(const int& channel);
  double getMeasuredVoltage(const int& channel);
};

}
