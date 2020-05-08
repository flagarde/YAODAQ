#include "CAENDigitizerConnector.hpp"

#include "CAENDigitizer.h"
#include "CAENDigitizerException.hpp"
#include "toml.hpp"

#include <iostream>

namespace CAEN
{

std::unordered_map<std::string, int>
    CAENDigitizerConnector::m_ConnectionTypeList{
        {"USB", CAEN_DGTZ_USB}, {"OPTICAL", CAEN_DGTZ_OpticalLink}};

CAENDigitizerConnector::CAENDigitizerConnector(): Connector("CAENDigitizer")
{
}

void CAENDigitizerConnector::DoConnect()
{
  CAENDigitizerException(CAEN_DGTZ_OpenDigitizer(
      static_cast<CAEN_DGTZ_ConnectionType>(
          m_ConnectionTypeList[m_ConnectionType]),
      m_LinkNum, m_ConetNode, m_VMEBaseAddress, &m_Handle));
}

void CAENDigitizerConnector::DoDisconnect()
{
  CAENDigitizerException(CAEN_DGTZ_CloseDigitizer(m_Handle));
}

void CAENDigitizerConnector::verifyParameters()
{
  try
  {
    m_ConnectionType =
        toml::find<std::string>(getParameters(), "Connection Type");
  }
  catch(const std::out_of_range& e)
  {
    std::cout << "\"Connection Type\" key not set !" << std::endl;
    std::exit(2);
  }
  if(m_ConnectionTypeList.find(m_ConnectionType) == m_ConnectionTypeList.end())
  {
    std::cout << "Connection Type " << m_ConnectionType << " Unknown !"
              << std::endl;
    std::exit(2);
  }
  try
  {
    m_LinkNum = toml::find<int>(getParameters(), "Link Number");
  }
  catch(const std::out_of_range& e)
  {
    std::cout << "\"Link Number\" key not set !" << std::endl;
    std::exit(2);
  }
  if(m_ConnectionType == "OPTICAL")
  {
    try
    {
      m_ConetNode = toml::find<int>(getParameters(), "Conet Node");
    }
    catch(const std::out_of_range& e)
    {
      std::cout << "\"Conet Node\" key not set !" << std::endl;
      std::exit(2);
    }
  }
  try
  {
    m_VMEBaseAddress = toml::find<int>(getParameters(), "VME Base Address");
  }
  catch(const std::out_of_range& e)
  {
    std::cout << "\"VME Base Address\" key not set !" << std::endl;
    std::exit(2);
  }
  if(m_VMEBaseAddress < 0 || m_VMEBaseAddress > 0xFFFF)
  {
    std::cout << "\"VME Base Address\" should be >0 and <0xFFFF !" << std::endl;
    std::exit(2);
  }
}

}  // namespace CAEN
