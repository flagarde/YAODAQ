#include "CAENDigitizerConnector.hpp"

#include "CAENDigitizer.h"
#include "CAENDigitizerException.hpp"
#include "Exception.hpp"
#include "StatusCode.hpp"
#include "toml.hpp"

using namespace yaodaq;

namespace CAEN
{
CAENDigitizerConnector::CAENDigitizerConnector(): Connector("CAENDigitizer") {}

void CAENDigitizerConnector::DoConnect()
{
  if(m_ConnectionType == "USB") CAENDigitizerException(CAEN_DGTZ_OpenDigitizer(CAEN_DGTZ_USB, m_LinkNum, m_ConetNode, m_VMEBaseAddress, &m_Handle));
  else
    CAENDigitizerException(CAEN_DGTZ_OpenDigitizer(CAEN_DGTZ_OpticalLink, m_LinkNum, m_ConetNode, m_VMEBaseAddress, &m_Handle));
}

void CAENDigitizerConnector::DoDisconnect()
{
  CAENDigitizerException(CAEN_DGTZ_CloseDigitizer(m_Handle));
}

void CAENDigitizerConnector::verifyParameters()
{
  try
  {
    m_ConnectionType = toml::find<std::string>(getParameters(), "Connection Type");
  }
  catch(const std::out_of_range& e)
  {
    throw Exception(StatusCode::NOT_FOUND, "\"Connection Type\" key not set !");
  }
  if(m_ConnectionType != "USB" && m_ConnectionType != "OPTICAL") { throw Exception(StatusCode::NOT_FOUND, "Connection Type \"{}\" Unknown ! Should be USB or OPTICAL !", m_ConnectionType); }
  try
  {
    m_LinkNum = toml::find<int>(getParameters(), "Link Number");
  }
  catch(const std::out_of_range& e)
  {
    throw Exception(StatusCode::NOT_FOUND, "\"Link Number\" key not set !");
  }
  if(m_ConnectionType == "OPTICAL")
  {
    try
    {
      m_ConetNode = toml::find<int>(getParameters(), "Conet Node");
    }
    catch(const std::out_of_range& e)
    {
      throw Exception(StatusCode::NOT_FOUND, "\"Conet Node\" key not set !");
    }
  }
  try
  {
    m_VMEBaseAddress = toml::find<int>(getParameters(), "VME Base Address");
  }
  catch(const std::out_of_range& e)
  {
    throw Exception(StatusCode::NOT_FOUND, "\"VME Base Address\" key not set !");
  }
  if(m_VMEBaseAddress < 0 || m_VMEBaseAddress > 0xFFFFFFFF) { throw Exception(StatusCode::OUT_OF_RANGE, "\"VME Base Address\" should be >0 and <0xFFFFFFFF !"); }
}

}  // namespace CAEN
