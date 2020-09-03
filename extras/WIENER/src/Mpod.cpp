#include "Mpod.hpp"

namespace WIENER
{

Mpod::Mpod(const std::string& name): Board(name, "Mpod") {}


double Mpod::getOutputRiseRate(int channel)
{
  /*if (channel < 0 || channel >= MaxChannelsPerCrate)
  {
    m_errorCode = SNMP_NOSUCHINSTANCE;
    return 0;
  }*/

  return snmpGetDouble(outputVoltageRiseRate[channel]);
}


}
