#include "CAENVMEConnector.hpp"
#include "CAENVMEError.hpp"
#include "CAENVMElib.h"
#include <iostream>
#include "get.hpp"
#include "parser.hpp"
#include "literal.hpp"
#include "serializer.hpp"

namespace CAEN 
{

CAENVMEConnector::CAENVMEConnector():Connector("CAENVME")
{
  
}

std::int32_t CAENVMEConnector::Connect()
{
  if(m_Model=="V1718") CAENVMEError(CAENVME_Init(cvV1718,toml::find<short>(m_Configs,"Link"),toml::find_or<short>(m_Configs,"Board Number",0),&m_Handle));
  else if (m_Model=="V2718") CAENVMEError(CAENVME_Init(cvV2718,toml::find<short>(m_Configs,"Link"),toml::find_or<short>(m_Configs,"Board Number",0),&m_Handle));
  else if (m_Model=="A2818") CAENVMEError(CAENVME_Init(cvA2818,toml::find<short>(m_Configs,"Link"),toml::find_or<short>(m_Configs,"Board Number",0),&m_Handle));
  else if (m_Model=="A2719") CAENVMEError(CAENVME_Init(cvA2719,toml::find<short>(m_Configs,"Link"),toml::find_or<short>(m_Configs,"Board Number",0),&m_Handle));
  else CAENVMEError(CAENVME_Init(cvA3818,toml::find<short>(m_Configs,"Link"),toml::find_or<short>(m_Configs,"Board Number",0),&m_Handle));
  return m_Handle;
}

void CAENVMEConnector::Disconnect()
{
  CAENVMEError(CAENVME_End(m_Handle));
}

void CAENVMEConnector::verifyConfig()
{
  try
  {
    m_Model=toml::find_or<std::string>(m_Configs,"Model","");
    if(m_Model!="V1718"&&m_Model!="V2718"&&m_Model!="A2818"&&m_Model!="A2719"&&m_Model!="A3818");
    {
      std::cout<<"Model should be V1718, V2718, A2818, A2719, A3818 !"<<std::endl;
    }
  }
  catch(const std::out_of_range& e)
  {
    std::cout<<"Model key not set !"<<std::endl;
    std::exit(2);
  }
  
  try
  {
    toml::find<short>(m_Configs,"Link");
  }
  catch(const std::out_of_range& e)
  {
    std::cout<<"Link key not set !"<<std::endl;
    std::exit(2);
  }
  if(m_Model=="A2818"||m_Model=="A2719"||m_Model=="A3818")
  {
    try
    {
      toml::find<short>(m_Configs,"Board Number");
    }
    catch(const std::out_of_range& e)
    {
      std::cout<<"\"Board Number\" key not set !"<<std::endl;
      std::exit(2);
    }
  }
}

}
