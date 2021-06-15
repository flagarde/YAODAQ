#pragma once
#include "Board.hpp"

namespace WIENER
{

class Mpod : public yaodaq::Board
{
public:
  Mpod(const std::string& name = "Mpod");
  void turnOnChannel();
  void turnOffChannel();



};

}
