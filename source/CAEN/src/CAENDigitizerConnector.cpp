#include "CAENDigitizerConnector.hpp"
#include "CAENDigitizerError.hpp"
#include "CAENDigitizer.h"
#include <iostream>
#include "get.hpp"
#include "parser.hpp"
#include "literal.hpp"


namespace CAEN 
{

std::unordered_map<std::string,int> CAENDigitizerConnector::m_ModelList
{
    {"V1724",CAEN_DGTZ_V1724},    /*!< \brief The board is V1724  */
    {"V1721",CAEN_DGTZ_V1721},    /*!< \brief The board is V1721  */
    {"V1731",CAEN_DGTZ_V1731},    /*!< \brief The board is V1731  */
    {"V1720",CAEN_DGTZ_V1720},    /*!< \brief The board is V1720  */
    {"V1740",CAEN_DGTZ_V1740},    /*!< \brief The board is V1740  */
    {"V1751",CAEN_DGTZ_V1751},    /*!< \brief The board is V1751  */
    {"DT5724",CAEN_DGTZ_DT5724},    /*!< \brief The board is DT5724 */
    {"DT5721",CAEN_DGTZ_DT5721},    /*!< \brief The board is DT5721 */
    {"DT5731",CAEN_DGTZ_DT5731},    /*!< \brief The board is DT5731 */
    {"DT5720",CAEN_DGTZ_DT5720},    /*!< \brief The board is DT5720 */
    {"DT5740",CAEN_DGTZ_DT5740},   /*!< \brief The board is DT5740 */
    {"DT5751",CAEN_DGTZ_DT5751},   /*!< \brief The board is DT5751 */
    {"N6724",CAEN_DGTZ_N6724},   /*!< \brief The board is N6724  */
    {"N6721",CAEN_DGTZ_N6721},   /*!< \brief The board is N6721  */
    {"N6731",CAEN_DGTZ_N6731},   /*!< \brief The board is N6731  */
    {"N6720",CAEN_DGTZ_N6720},   /*!< \brief The board is N6720  */
    {"N6740",CAEN_DGTZ_N6740},   /*!< \brief The board is N6740  */
    {"N6751",CAEN_DGTZ_N6751},   /*!< \brief The board is N6751  */
    {"DT5742",CAEN_DGTZ_DT5742},   /*!< \brief The board is DT5742 */
    {"N6742",CAEN_DGTZ_N6742},   /*!< \brief The board is N6742  */
    {"V1742",CAEN_DGTZ_V1742},   /*!< \brief The board is V1742  */
    {"DT5780",CAEN_DGTZ_DT5780},   /*!< \brief The board is DT5780 */
    {"N6780",CAEN_DGTZ_N6780},   /*!< \brief The board is N6780  */
    {"V1780",CAEN_DGTZ_V1780},   /*!< \brief The board is V1780  */
    {"DT5761",CAEN_DGTZ_DT5761},   /*!< \brief The board is DT5761 */
    {"N6761",CAEN_DGTZ_N6761},   /*!< \brief The board is N6761  */
    {"V1761",CAEN_DGTZ_V1761},   /*!< \brief The board is V1761  */
    {"DT5743",CAEN_DGTZ_DT5743},   /*!< \brief The board is DT5743 */
    {"N6743",CAEN_DGTZ_N6743},   /*!< \brief The board is N6743  */
    {"V1743",CAEN_DGTZ_V1743},   /*!< \brief The board is V1743  */
    {"DT5730",CAEN_DGTZ_DT5730},   /*!< \brief The board is DT5730 */
    {"N6730",CAEN_DGTZ_N6730},   /*!< \brief The board is N6730  */
    {"V1730",CAEN_DGTZ_V1730},   /*!< \brief The board is V1730  */
    {"DT5790",CAEN_DGTZ_DT5790},   /*!< \brief The board is DT5790 */
    {"N6790",CAEN_DGTZ_N6790},   /*!< \brief The board is N6790  */
    {"V1790",CAEN_DGTZ_V1790},   /*!< \brief The board is V1790  */
    {"DT5781",CAEN_DGTZ_DT5781},   /*!< \brief The board is DT5781 */
    {"N6781",CAEN_DGTZ_N6781},   /*!< \brief The board is N6781  */
    {"V1781",CAEN_DGTZ_V1781},   /*!< \brief The board is V1781  */
    {"DT5725",CAEN_DGTZ_DT5725},   /*!< \brief The board is DT5725 */
    {"N6725",CAEN_DGTZ_N6725},   /*!< \brief The board is N6725  */
    {"V1725",CAEN_DGTZ_V1725},   /*!< \brief The board is V1725  */
    {"V1782",CAEN_DGTZ_V1782}   /*!< \brief The board is V1782  */
};
  
std::unordered_map<std::string,int> CAENDigitizerConnector::m_ConnectionTypeList
{
  {"USB",CAEN_DGTZ_USB},
  {"OPTICAL",CAEN_DGTZ_OpticalLink}
};
  
CAENDigitizerConnector::CAENDigitizerConnector(const ConnectorInfos& infos):Connector("CAENDigitizer",infos)
{
  
}

std::int32_t CAENDigitizerConnector::Connect()
{
  CAENDigitizerError(CAEN_DGTZ_OpenDigitizer(static_cast<CAEN_DGTZ_ConnectionType>(m_ConnectionTypeList[m_ConnectionType]),m_LinkNum,m_ConetNode,m_VMEBaseAddress,&m_Handle));
  return m_Handle;
}

void CAENDigitizerConnector::Disconnect()
{
  CAENDigitizerError(CAEN_DGTZ_CloseDigitizer(m_Handle));
}

void CAENDigitizerConnector::verifyParameters()
{  
  try
  {
    m_ConnectionType=toml::find<std::string>(getParameters(),"Connection Type");
  }
  catch(const std::out_of_range& e)
  {
    std::cout<<"\"Connection Type\" key not set !"<<std::endl;
    std::exit(2);
  }
  if(m_ConnectionTypeList.find(m_ConnectionType)==m_ConnectionTypeList.end())
  {
    std::cout<<"Connection Type "<<m_ConnectionType<<" Unknown !"<<std::endl;
    std::exit(2);
  }
  try
  {
    m_LinkNum=toml::find<int>(getParameters(),"Link Number");
  }
  catch(const std::out_of_range& e)
  {
    std::cout<<"\"Link Number\" key not set !"<<std::endl;
    std::exit(2);
  }
  if(m_ConnectionType=="OPTICAL")
  {
    try
    { 
      m_ConetNode=toml::find<int>(getParameters(),"Conet Node");
    }
    catch(const std::out_of_range& e)
    {
      std::cout<<"\"Conet Node\" key not set !"<<std::endl;
      std::exit(2);
    }
  }
  try
  { 
    m_VMEBaseAddress=toml::find<int>(getParameters(),"VME Base Address");
  }
  catch(const std::out_of_range& e)
  {
    std::cout<<"\"VME Base Address\" key not set !"<<std::endl;
    std::exit(2);
  }
  if(m_VMEBaseAddress<0||m_VMEBaseAddress>0xFFFF)
  {
    std::cout<<"\"VME Base Address\" should be >0 and <0xFFFF !"<<std::endl;
    std::exit(2);
  }
}

}
