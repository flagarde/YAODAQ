#include "CAENVMEConnector.hpp"
#include "CAENVMEError.hpp"
#include "CAENVMElib.h"
#include <iostream>
#include "toml.hpp"

namespace CAEN 
{

std::unordered_map<std::string,int> CAENVMEConnector::m_ModelList
{
    {"V1718",cvV1718},    /*!< \brief The board is V1718  */
    {"V2718",cvV2718},    /*!< \brief The board is V2718  */
    {"A2818",cvA2818},    /*!< \brief The board is A2818  */
    {"A2719",cvA2719},    /*!< \brief The board is A2719  */
    {"A3818",cvA3818},    /*!< \brief The board is A3818  */
};  
  
  
CAENVMEConnector::CAENVMEConnector(const ConnectorInfos& infos):Connector("CAENVME",infos)
{

}

std::int32_t CAENVMEConnector::Connect()
{
  CAENVMEError(CAENVME_Init(static_cast<CVBoardTypes>(m_ModelList[m_Model]),m_LinkNumber,m_ConetNode,&m_Handle));
  return m_Handle;
}

void CAENVMEConnector::Disconnect()
{
  CAENVMEError(CAENVME_End(m_Handle));
}

void CAENVMEConnector::verifyParameters()
{
  try
  {
    m_Model=toml::find_or<std::string>(getParameters(),"Model","");
    if(m_ModelList.find(m_Model)==m_ModelList.end())
    {
      std::cout<<"Model "<<m_Model<<" Unknown !"<<std::endl;
      std::exit(2);
    }
  }
  catch(const std::out_of_range& e)
  {
    std::cout<<"Model key not set !"<<std::endl;
    std::exit(2);
  }
  try
  {
    m_LinkNumber=toml::find<short>(getParameters(),"Link Number");
  }
  catch(const std::out_of_range& e)
  {
    std::cout<<"\"Link Number\" key not set !"<<std::endl;
    std::exit(2);
  }
  if(m_Model=="A2818"||m_Model=="A2719"||m_Model=="A3818")
  {
    try
    {
      m_ConetNode=toml::find<short>(getParameters(),"Conet Node");
    }
    catch(const std::out_of_range& e)
    {
      std::cout<<"\"Board Number\" key not set !"<<std::endl;
      std::exit(2);
    }
  }
}


}
