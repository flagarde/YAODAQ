#pragma once
#include "Board.hpp"

namespace WIENER
{


class Mpod: public Board
{
public:
  Mpod(const std::string& name = "");
  /**
   * @brief Returns the channel voltage rise rate in Volt/second.
   *
   * @note This function is for WIENER LV only.
   * For iseg HV modules, use getModuleRampSpeedVoltage().
   * @param session The handle returned by snmpOpen()
   * @param channel The requested channel (0...::MaxChannelsPerCrate - 1)
   * @return
   */
  double getOutputRiseRate(int channel);


private:
};

}
