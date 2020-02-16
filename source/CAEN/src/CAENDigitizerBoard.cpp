#include "CAENDigitizerBoard.hpp"
#include "CAENDigitizerError.hpp"
#include "CAENDigitizer.h"

namespace CAEN
{

class EventInfo
{
public:
  EventInfo(const CAEN_DGTZ_EventInfo_t& event)
  {
    t=event;
  }
  std::uint32_t getEventSize()
  {
    return t.EventSize;
  }
  std::uint32_t getBoardId()
  {
    return t.BoardId;
  }
  std::uint32_t getPattern()
  {
    return t.Pattern;
  }
  std::uint32_t getChannelMask()
  {
    return t.ChannelMask;
  }
  std::uint32_t getEventCounter()
  {
    return t.EventCounter;
  }
  std::uint32_t getTriggerTimeTag()
  {
    return t.TriggerTimeTag;
  }
private:
  CAEN_DGTZ_EventInfo_t t;
};

CAENDigitizerBoard::CAENDigitizerBoard(const std::string& type, const std::string& name):Board(type,name)
{
    
}

EventInfo CAENDigitizerBoard::GetEventInfo(const std::int32_t& numEvent)
{
  CAEN_DGTZ_EventInfo_t toto;
  char* p=m_EventPtr.get();
  CAENDigitizerError(CAEN_DGTZ_GetEventInfo(m_Handle,m_Buffer.get(),m_BufferSize,numEvent,&toto,&p));
  EventInfo ret(toto);
  return ret;
}

void CAENDigitizerBoard::WriteRegister(const std::uint32_t& Address,const std::uint32_t& Data)
{
  CAENDigitizerError(CAEN_DGTZ_WriteRegister(m_Handle,Address,Data));
}

std::uint32_t CAENDigitizerBoard::ReadRegister(const std::uint32_t& Address)
{
  std::uint32_t ret{0};
  CAENDigitizerError(CAEN_DGTZ_ReadRegister(m_Handle,Address,&ret));
  return ret;
}

void CAENDigitizerBoard::GetInfo()
{
  CAEN_DGTZ_BoardInfo_t boardinfo;
  CAENDigitizerError(CAEN_DGTZ_GetInfo(m_Handle,&boardinfo)); 
  setModelName(boardinfo.ModelName);
  setModel(boardinfo.Model);
  setNbrChannels(boardinfo.Channels);
  setFormFactor(boardinfo.FormFactor);
  setFamilyCode(boardinfo.FamilyCode);
  setROCFirmwareRel(boardinfo.ROC_FirmwareRel);
  setAMCFirmwareRel(boardinfo.AMC_FirmwareRel);
  setSerialNumber(boardinfo.SerialNumber);
  setMezzanineSerialNumber(boardinfo.MezzanineSerNum);
  setPCBRevision(boardinfo.PCB_Revision);
  setADCNBits(boardinfo.ADC_NBits);
  setSAMCorrectionDataLoaded(boardinfo.SAMCorrectionDataLoaded);
  setLicense(boardinfo.License);
  setCommHandle(boardinfo.CommHandle);
  setVMEHandle(boardinfo.VMEHandle);
}

void CAENDigitizerBoard::setModelName(const std::string& name)
{
  m_ModelName=name;
}

void CAENDigitizerBoard::setModel(const std::uint32_t& model)
{
  switch(model) 
  {
    case CAEN_DGTZ_V1724 : m_Model="V1724"; break; /*!< \brief The board is V1724  */
    case CAEN_DGTZ_V1721 : m_Model="V1721"; break; /*!< \brief The board is V1721  */
    case CAEN_DGTZ_V1731 : m_Model="V1731"; break; /*!< \brief The board is V1731  */
    case CAEN_DGTZ_V1720 : m_Model="V1720"; break; /*!< \brief The board is V1720  */
    case CAEN_DGTZ_V1740 : m_Model="V1740"; break; /*!< \brief The board is V1740  */
    case CAEN_DGTZ_V1751 : m_Model="V1751"; break; /*!< \brief The board is V1751  */
    case CAEN_DGTZ_DT5724 : m_Model="DT5724"; break; /*!< \brief The board is DT5724 */
    case CAEN_DGTZ_DT5721 : m_Model="DT5721"; break; /*!< \brief The board is DT5721 */
    case CAEN_DGTZ_DT5731 : m_Model="DT5731"; break; /*!< \brief The board is DT5731 */
    case CAEN_DGTZ_DT5720 : m_Model="DT5720"; break; /*!< \brief The board is DT5720 */
    case CAEN_DGTZ_DT5740 : m_Model="DT5740"; break; /*!< \brief The board is DT5740 */
    case CAEN_DGTZ_DT5751 : m_Model="DT5751"; break; /*!< \brief The board is DT5751 */
    case CAEN_DGTZ_N6724 : m_Model="N6724"; break; /*!< \brief The board is N6724  */
    case CAEN_DGTZ_N6721 : m_Model="N6721"; break; /*!< \brief The board is N6721  */
    case CAEN_DGTZ_N6731 : m_Model="N6731"; break; /*!< \brief The board is N6731  */
    case CAEN_DGTZ_N6720 : m_Model="N6720"; break; /*!< \brief The board is N6720  */
    case CAEN_DGTZ_N6740 : m_Model="N6740"; break; /*!< \brief The board is N6740  */
    case CAEN_DGTZ_N6751 : m_Model="N6751"; break; /*!< \brief The board is N6751  */
    case CAEN_DGTZ_DT5742 : m_Model="DT5742"; break; /*!< \brief The board is DT5742 */
    case CAEN_DGTZ_N6742 : m_Model="N6742"; break; /*!< \brief The board is N6742  */
    case CAEN_DGTZ_V1742 : m_Model="V1742"; break; /*!< \brief The board is V1742  */
    case CAEN_DGTZ_DT5780 : m_Model="DT5780"; break; /*!< \brief The board is DT5780 */
    case CAEN_DGTZ_N6780 : m_Model="N6780"; break; /*!< \brief The board is N6780  */
    case CAEN_DGTZ_V1780 : m_Model="V1780"; break; /*!< \brief The board is V1780  */
    case CAEN_DGTZ_DT5761 : m_Model="DT5761"; break; /*!< \brief The board is DT5761 */
    case CAEN_DGTZ_N6761 : m_Model="N6761"; break; /*!< \brief The board is N6761  */
    case CAEN_DGTZ_V1761 : m_Model="V1761"; break; /*!< \brief The board is V1761  */
    case CAEN_DGTZ_DT5743 : m_Model="DT5743"; break; /*!< \brief The board is DT5743 */
    case CAEN_DGTZ_N6743 : m_Model="N6743"; break; /*!< \brief The board is N6743  */
    case CAEN_DGTZ_V1743 : m_Model="V1743"; break; /*!< \brief The board is V1743  */
    case CAEN_DGTZ_DT5730 : m_Model="DT5730"; break; /*!< \brief The board is DT5730 */
    case CAEN_DGTZ_N6730 : m_Model="N6730"; break; /*!< \brief The board is N6730  */
    case CAEN_DGTZ_V1730 : m_Model="V1730"; break; /*!< \brief The board is V1730  */
    case CAEN_DGTZ_DT5790 : m_Model="DT5790"; break; /*!< \brief The board is DT5790 */
    case CAEN_DGTZ_N6790 : m_Model="N6790"; break; /*!< \brief The board is N6790  */
    case CAEN_DGTZ_V1790 : m_Model="V1790"; break; /*!< \brief The board is V1790  */
    case CAEN_DGTZ_DT5781 : m_Model="DT5791"; break; /*!< \brief The board is DT5781 */
    case CAEN_DGTZ_N6781 : m_Model="N6781"; break; /*!< \brief The board is N6781  */
    case CAEN_DGTZ_V1781 : m_Model="V1781"; break; /*!< \brief The board is V1781  */
    case CAEN_DGTZ_DT5725 : m_Model="DT5725"; break; /*!< \brief The board is DT5725 */
    case CAEN_DGTZ_N6725 : m_Model="N6725"; break; /*!< \brief The board is N6725  */
    case CAEN_DGTZ_V1725 : m_Model="V1725"; break; /*!< \brief The board is V1725  */
    case CAEN_DGTZ_V1782 : m_Model="V1782"; break; /*!< \brief The board is V1782  */
    default : m_Model="";
  }
}

void CAENDigitizerBoard::setNbrChannels(const std::uint32_t& nbr)
{
  m_NbrChannels=nbr;
}

void CAENDigitizerBoard::setFormFactor(const std::uint32_t& form)
{
  switch(form) 
  {   
    case CAEN_DGTZ_VME64_FORM_FACTOR : m_FormFactor="VME64"; break;
    case CAEN_DGTZ_VME64X_FORM_FACTOR : m_FormFactor="VME64X"; break;
    case CAEN_DGTZ_DESKTOP_FORM_FACTOR : m_FormFactor="DESKTOP"; break;
    case CAEN_DGTZ_NIM_FORM_FACTOR : m_FormFactor="NIM"; break;
    default : m_FormFactor="";
  }
}

void CAENDigitizerBoard::setFamilyCode(const std::uint32_t& fam)
{
  switch(fam) 
  {   
    case CAEN_DGTZ_XX724_FAMILY_CODE : m_FamilyCode="XX724"; break;
    case CAEN_DGTZ_XX721_FAMILY_CODE : m_FamilyCode="XX721"; break;
    case CAEN_DGTZ_XX731_FAMILY_CODE : m_FamilyCode="XX731"; break;
    case CAEN_DGTZ_XX720_FAMILY_CODE : m_FamilyCode="XX720"; break;
    case CAEN_DGTZ_XX740_FAMILY_CODE : m_FamilyCode="XX740"; break;  
    case CAEN_DGTZ_XX751_FAMILY_CODE : m_FamilyCode="XX751"; break;
    case CAEN_DGTZ_XX742_FAMILY_CODE : m_FamilyCode="XX742"; break;
    case CAEN_DGTZ_XX780_FAMILY_CODE : m_FamilyCode="XX780"; break;
    case CAEN_DGTZ_XX761_FAMILY_CODE : m_FamilyCode="XX761"; break;
    case CAEN_DGTZ_XX743_FAMILY_CODE : m_FamilyCode="XX743"; break;
    // NOTE: 10 is skipped because maps family models DT55xx
    case CAEN_DGTZ_XX730_FAMILY_CODE : m_FamilyCode="XX730"; break;
    case CAEN_DGTZ_XX790_FAMILY_CODE : m_FamilyCode="XX790"; break;
    case CAEN_DGTZ_XX781_FAMILY_CODE : m_FamilyCode="XX781"; break;
    case CAEN_DGTZ_XX725_FAMILY_CODE : m_FamilyCode="XX725"; break;
	case CAEN_DGTZ_XX782_FAMILY_CODE : m_FamilyCode="XX782"; break;
    default : m_FamilyCode="";
  }  
}

void CAENDigitizerBoard::setROCFirmwareRel(const std::string& firm)
{
  m_ROC_FirmwareRel=firm;
}

void CAENDigitizerBoard::setAMCFirmwareRel(const std::string& firm)
{
  m_AMC_FirmwareRel=firm;
}

void CAENDigitizerBoard::setSerialNumber(const std::uint32_t& serial)
{
  m_SerialNumber=serial;
}
  
void CAENDigitizerBoard::setMezzanineSerialNumber(char serials[4][8])
{
  m_MezzanineSerialNumber[0]=serials[0];
  m_MezzanineSerialNumber[1]=serials[1];
  m_MezzanineSerialNumber[2]=serials[2];
  m_MezzanineSerialNumber[3]=serials[3];
}
 
void CAENDigitizerBoard::setPCBRevision(const std::uint32_t& pcb)
{
  m_PCB_Revision=pcb;
}
  
void CAENDigitizerBoard::setADCNBits(const std::uint32_t& nbits)
{
  m_ADC_NBits=nbits;
}
  
void CAENDigitizerBoard::setSAMCorrectionDataLoaded(const std::uint32_t& sam)
{
  m_SAMCorrectionDataLoaded=sam;
}
  
void CAENDigitizerBoard::setLicense(const std::string& lic)
{
  m_License=lic;
}
  
void CAENDigitizerBoard::setCommHandle(const std::uint32_t& han)
{
  m_CommHandle=han;
}
  
void CAENDigitizerBoard::setVMEHandle(const std::uint32_t& han)
{
  m_VMEHandle=han;
}

void CAENDigitizerBoard::Reset()
{
  CAENDigitizerError(CAEN_DGTZ_Reset(m_Handle));
}

void CAENDigitizerBoard::ClearData()
{
  CAENDigitizerError(CAEN_DGTZ_ClearData(m_Handle));
}

void CAENDigitizerBoard::SendSWtrigger()
{
  CAENDigitizerError(CAEN_DGTZ_SendSWtrigger(m_Handle));
}

void CAENDigitizerBoard::SWStartAcquisition()
{
  CAENDigitizerError(CAEN_DGTZ_SWStartAcquisition(m_Handle));
}

void CAENDigitizerBoard::SWStopAcquisition()
{
  CAENDigitizerError(CAEN_DGTZ_SWStopAcquisition(m_Handle));
}

void CAENDigitizerBoard::IRQWait(const std::uint32_t& timeout)
{
  CAENDigitizerError(CAEN_DGTZ_IRQWait(m_Handle,timeout));
}

void CAENDigitizerBoard::SetDESMode(const std::string& on)
{
  if(on=="ENABLE") CAENDigitizerError(CAEN_DGTZ_SetDESMode(m_Handle,CAEN_DGTZ_ENABLE));
  else if(on=="DISABLE") CAENDigitizerError(CAEN_DGTZ_SetDESMode(m_Handle,CAEN_DGTZ_DISABLE));
  else throw CAENDigitizerError(CAEN_DGTZ_InvalidParam);
}

std::string CAENDigitizerBoard::GetDESMode()
{
  CAEN_DGTZ_EnaDis_t enable;
  CAENDigitizerError(CAEN_DGTZ_GetDESMode(m_Handle,&enable));
  if(enable==CAEN_DGTZ_ENABLE) return "ENABLE";
  else return "DISABLE";
}

void CAENDigitizerBoard::Calibrate()
{
  CAENDigitizerError(CAEN_DGTZ_Calibrate(m_Handle));
}

std::uint32_t CAENDigitizerBoard::ReadTemperature(const std::int32_t& ch)
{
  std::uint32_t temp{0};
  CAENDigitizerError(CAEN_DGTZ_ReadTemperature(m_Handle,ch,&temp));
  return temp;
}

std::string CAENDigitizerBoard::GetDPPFirmwareType()
{
  CAEN_DGTZ_DPPFirmware_t firmware;  
  CAENDigitizerError(CAEN_DGTZ_GetDPPFirmwareType(m_Handle,&firmware));
  switch(firmware)
  {
    case CAEN_DGTZ_DPPFirmware_PHA : return "PHA";
    case CAEN_DGTZ_DPPFirmware_PSD : return "PSD";
    case CAEN_DGTZ_DPPFirmware_CI : return "CI";
    case CAEN_DGTZ_DPPFirmware_ZLE : return "ZLE";
    case CAEN_DGTZ_DPPFirmware_QDC : return "QDC";
    case CAEN_DGTZ_DPPFirmware_DAW : return "DAW";
    case CAEN_DGTZ_NotDPPFirmware : return "NODPP";
    default : return "NODPP";
  }
}

void CAENDigitizerBoard::RearmInterrupt()
{
  CAENDigitizerError(CAEN_DGTZ_RearmInterrupt(m_Handle));
}

std::string CAENDigitizerBoard::GetTriggerPolarity(const std::uint32_t& channel)
{
  CAEN_DGTZ_TriggerPolarity_t polarity;
  CAENDigitizerError(CAEN_DGTZ_GetTriggerPolarity(m_Handle,channel,&polarity));
  if(channel==CAEN_DGTZ_TriggerOnRisingEdge) return "TriggerOnRisingEdge";
  else return "TriggerOnFallingEdge";
}

void CAENDigitizerBoard::SetTriggerPolarity(const std::uint32_t& channel,const std::string& Polarity)
{
  if(Polarity=="TriggerOnRisingEdge") CAENDigitizerError(CAEN_DGTZ_SetTriggerPolarity(m_Handle,channel,CAEN_DGTZ_TriggerOnRisingEdge));
  else if(Polarity=="TriggerOnFallingEdge") CAENDigitizerError(CAEN_DGTZ_SetTriggerPolarity(m_Handle,channel,CAEN_DGTZ_TriggerOnFallingEdge));
  else throw CAENDigitizerError(CAEN_DGTZ_InvalidParam);
  
}

std::string CAENDigitizerBoard::CAENDigitizerBoard::GetIOLevel()
{
  CAEN_DGTZ_IOLevel_t level;
  CAENDigitizerError(CAEN_DGTZ_GetIOLevel(m_Handle,&level));
  if(level==CAEN_DGTZ_IOLevel_NIM) return "NIM";
  else return "TTL";
}

void CAENDigitizerBoard::SetIOLevel(const std::string& level)
{
  if(level=="NIM") CAENDigitizerError(CAEN_DGTZ_SetIOLevel(m_Handle,CAEN_DGTZ_IOLevel_NIM));
  else if(level=="TTL") CAENDigitizerError(CAEN_DGTZ_SetIOLevel(m_Handle,CAEN_DGTZ_IOLevel_TTL));
  else throw CAENDigitizerError(CAEN_DGTZ_InvalidParam);
}

std::string CAENDigitizerBoard::VirtualProbeName(const int& probe)
{
  char name[256];
  CAENDigitizerError(CAEN_DGTZ_GetDPP_VirtualProbeName(probe,name));
  return name;
}

void CAENDigitizerBoard::SetAcquisitionMode(const std::string& mode)
{
  if(mode=="SW_CONTROLLED") CAENDigitizerError(CAEN_DGTZ_SetAcquisitionMode(m_Handle,CAEN_DGTZ_SW_CONTROLLED));
  else if(mode=="IN_CONTROLLED") CAENDigitizerError(CAEN_DGTZ_SetAcquisitionMode(m_Handle,CAEN_DGTZ_S_IN_CONTROLLED));
  else if(mode=="FIRST_TRG_CONTROLLED")  CAENDigitizerError(CAEN_DGTZ_SetAcquisitionMode(m_Handle,CAEN_DGTZ_FIRST_TRG_CONTROLLED));
  else if(mode=="LVDS_CONTROLLED") CAENDigitizerError(CAEN_DGTZ_SetAcquisitionMode(m_Handle,CAEN_DGTZ_LVDS_CONTROLLED));
  else throw CAENDigitizerError(CAEN_DGTZ_InvalidParam);
}

std::string CAENDigitizerBoard::GetAcquisitionMode()
{
  CAEN_DGTZ_AcqMode_t mode;
  CAENDigitizerError(CAEN_DGTZ_GetAcquisitionMode(m_Handle,&mode));
  switch(mode)
  {
    case CAEN_DGTZ_SW_CONTROLLED : return "SW_CONTROLLED";
    case CAEN_DGTZ_S_IN_CONTROLLED : return "S_IN_CONTROLLED";
    case CAEN_DGTZ_FIRST_TRG_CONTROLLED : return "FIRST_TRG_CONTROLLED";
    default : return "LVDS_CONTROLLED";
  }
}

void CAENDigitizerBoard::SetPostTriggerSize(const std::uint32_t& percent)
{
  CAENDigitizerError(CAEN_DGTZ_SetPostTriggerSize(m_Handle,percent));
}

std::uint32_t CAENDigitizerBoard::GetPostTriggerSize()
{
  std::uint32_t percent{0};
  CAENDigitizerError(CAEN_DGTZ_GetPostTriggerSize(m_Handle,&percent));
  return percent;
}

void CAENDigitizerBoard::MallocReadoutBuffer()
{
  char* ptr = m_Buffer.get();
  CAENDigitizerError(CAEN_DGTZ_MallocReadoutBuffer(m_Handle,&ptr,&m_AllocatedSize));
}

void CAENDigitizerBoard::FreeReadoutBuffer()
{
  char* ptr = m_Buffer.get();
  CAENDigitizerError(CAEN_DGTZ_FreeReadoutBuffer(&ptr));
}

void CAENDigitizerBoard::ReadData(const std::string& mode)
{
  if(mode=="SLAVE_TERMINATED_READOUT_MBLT")
  CAENDigitizerError(CAEN_DGTZ_ReadData(m_Handle,CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT,m_Buffer.get(),&m_BufferSize));
  else if(mode=="SLAVE_TERMINATED_READOUT_2eVME")CAENDigitizerError(CAEN_DGTZ_ReadData(m_Handle,CAEN_DGTZ_SLAVE_TERMINATED_READOUT_2eVME,m_Buffer.get(),&m_BufferSize));
  else if(mode=="SLAVE_TERMINATED_READOUT_2eSST") CAENDigitizerError(CAEN_DGTZ_ReadData(m_Handle,CAEN_DGTZ_SLAVE_TERMINATED_READOUT_2eSST,m_Buffer.get(),&m_BufferSize));
  else if(mode=="POLLING_MBLT")CAENDigitizerError(CAEN_DGTZ_ReadData(m_Handle,CAEN_DGTZ_POLLING_MBLT,m_Buffer.get(),&m_BufferSize));
  else if(mode=="POLLING_2eVME")CAENDigitizerError(CAEN_DGTZ_ReadData(m_Handle,CAEN_DGTZ_POLLING_2eVME,m_Buffer.get(),&m_BufferSize));
  else if(mode=="POLLING_2eSST")CAENDigitizerError(CAEN_DGTZ_ReadData(m_Handle,CAEN_DGTZ_POLLING_2eSST,m_Buffer.get(),&m_BufferSize));
  else throw CAENDigitizerError(CAEN_DGTZ_InvalidParam);
}

void CAENDigitizerBoard::SetChannelPulsePolarity(const std::uint32_t& channel,const std::string& pol)
{
   if(pol=="POSITIVE")CAENDigitizerError(CAEN_DGTZ_SetChannelPulsePolarity(m_Handle,channel,CAEN_DGTZ_PulsePolarityPositive));
   else if(pol=="NEGATIVE")CAENDigitizerError(CAEN_DGTZ_SetChannelPulsePolarity(m_Handle,channel,CAEN_DGTZ_PulsePolarityNegative));
   else throw CAENDigitizerError(CAEN_DGTZ_InvalidParam);
}

std::string CAENDigitizerBoard::GetChannelPulsePolarity(const std::uint32_t& channel)
{
  CAEN_DGTZ_PulsePolarity_t pol;
  CAENDigitizerError(CAEN_DGTZ_GetChannelPulsePolarity(m_Handle,channel,&pol));
  if(pol==CAEN_DGTZ_PulsePolarityPositive) return "POSITIVE";
  else return "NEGATIVE";
}

void CAENDigitizerBoard::SetGroupTriggerThreshold(const std::uint32_t& group,const std::uint32_t& Tvalue)
{
  CAENDigitizerError(CAEN_DGTZ_SetGroupTriggerThreshold(m_Handle,group,Tvalue));
}

std::uint32_t CAENDigitizerBoard::GetGroupTriggerThreshold(const std::uint32_t& group)
{
  std::uint32_t Tvalue{0};
  CAENDigitizerError(CAEN_DGTZ_GetGroupTriggerThreshold(m_Handle,group,&Tvalue));
  return Tvalue;
}

void CAENDigitizerBoard::DisableEventAlignedReadout()
{
  CAENDigitizerError(CAEN_DGTZ_DisableEventAlignedReadout(m_Handle));
}

void CAENDigitizerBoard::SetChannelDCOffset(const std::uint32_t& channel,const std::uint32_t& Tvalue)
{
  CAENDigitizerError(CAEN_DGTZ_SetChannelDCOffset(m_Handle,channel,Tvalue));
}

std::uint32_t CAENDigitizerBoard::GetChannelDCOffset(const std::uint32_t& channel)
{
  std::uint32_t Tvalue{0};
  CAENDigitizerError(CAEN_DGTZ_GetChannelDCOffset(m_Handle,channel,&Tvalue));
  return Tvalue;
}

void CAENDigitizerBoard::SetGroupDCOffset(const std::uint32_t& group,const std::uint32_t& Tvalue)
{
  CAENDigitizerError(CAEN_DGTZ_SetGroupDCOffset(m_Handle,group,Tvalue));
}

std::uint32_t CAENDigitizerBoard::GetGroupDCOffset(const std::uint32_t& group)
{
  std::uint32_t Tvalue{0};
  CAENDigitizerError(CAEN_DGTZ_GetGroupDCOffset(m_Handle,group,&Tvalue));
  return Tvalue;
}

void CAENDigitizerBoard::SetChannelTriggerThreshold(const std::uint32_t& channel,const std::uint32_t& Tvalue)
{
  CAENDigitizerError(CAEN_DGTZ_SetChannelTriggerThreshold(m_Handle,channel,Tvalue));
}

std::uint32_t CAENDigitizerBoard::GetChannelTriggerThreshold(const std::uint32_t& channel)
{
  std::uint32_t Tvalue{0};
  CAENDigitizerError(CAEN_DGTZ_GetChannelTriggerThreshold(m_Handle,channel,&Tvalue));
  return Tvalue;
}

void CAENDigitizerBoard::SetZeroSuppressionMode(const std::string& mode)
{
  if(mode=="NO")CAENDigitizerError(CAEN_DGTZ_SetZeroSuppressionMode(m_Handle,CAEN_DGTZ_ZS_NO));
  else if(mode=="INT")CAENDigitizerError(CAEN_DGTZ_SetZeroSuppressionMode(m_Handle,CAEN_DGTZ_ZS_INT));
  else if(mode=="ZLE")CAENDigitizerError(CAEN_DGTZ_SetZeroSuppressionMode(m_Handle,CAEN_DGTZ_ZS_ZLE));
  else if(mode=="AMP")CAENDigitizerError(CAEN_DGTZ_SetZeroSuppressionMode(m_Handle,CAEN_DGTZ_ZS_AMP));
  else throw CAENDigitizerError(CAEN_DGTZ_InvalidParam);
}

std::string CAENDigitizerBoard::GetZeroSuppressionMode()
{
  CAEN_DGTZ_ZS_Mode_t mode;
  CAENDigitizerError(CAEN_DGTZ_GetZeroSuppressionMode(m_Handle,&mode));
  switch(mode)
  {
    case CAEN_DGTZ_ZS_NO : return "NO";
    case CAEN_DGTZ_ZS_INT : return "INT";
    case CAEN_DGTZ_ZS_ZLE : return "ZLE";
    default : return "AMP";
  }
}

void CAENDigitizerBoard::SetRunSynchronizationMode(const std::string& mode)
{
  if(mode=="Disabled") 
  CAENDigitizerError(CAEN_DGTZ_SetRunSynchronizationMode(m_Handle,CAEN_DGTZ_RUN_SYNC_Disabled));
  else if(mode=="TrgOutTrgInDaisyChain")
  CAENDigitizerError(CAEN_DGTZ_SetRunSynchronizationMode(m_Handle,CAEN_DGTZ_RUN_SYNC_TrgOutTrgInDaisyChain));
  else if(mode=="TrgOutSinDaisyChain")
  CAENDigitizerError(CAEN_DGTZ_SetRunSynchronizationMode(m_Handle,CAEN_DGTZ_RUN_SYNC_TrgOutSinDaisyChain));
  else if(mode=="SinFanout")
  CAENDigitizerError(CAEN_DGTZ_SetRunSynchronizationMode(m_Handle,CAEN_DGTZ_RUN_SYNC_SinFanout));
  else if(mode=="CAEN_DGTZ_RUN_SYNC_GpioGpioDaisyChain")
  CAENDigitizerError(CAEN_DGTZ_SetRunSynchronizationMode(m_Handle,CAEN_DGTZ_RUN_SYNC_GpioGpioDaisyChain));
  else throw CAENDigitizerError(CAEN_DGTZ_InvalidParam);
}

std::string CAENDigitizerBoard::GetRunSynchronizationMode()
{
  CAEN_DGTZ_RunSyncMode_t mode;
  CAENDigitizerError(CAEN_DGTZ_GetRunSynchronizationMode(m_Handle,&mode));
  switch(mode)
  {
    case CAEN_DGTZ_RUN_SYNC_Disabled : return "Disabled";
    case CAEN_DGTZ_RUN_SYNC_TrgOutTrgInDaisyChain : return "TrgOutTrgInDaisyChain";
    case CAEN_DGTZ_RUN_SYNC_TrgOutSinDaisyChain : return "TrgOutSinDaisyChain";
    case CAEN_DGTZ_RUN_SYNC_SinFanout : return "SinFanout";
    default : return "GpioGpioDaisyChain";
  }
}

void CAENDigitizerBoard::SetEventPackaging(const std::string& on)
{
  if(on=="ENABLE") CAENDigitizerError(CAEN_DGTZ_SetEventPackaging(m_Handle,CAEN_DGTZ_ENABLE));
  else if(on=="DISABLE") CAENDigitizerError(CAEN_DGTZ_SetEventPackaging(m_Handle,CAEN_DGTZ_DISABLE));
  else throw CAENDigitizerError(CAEN_DGTZ_InvalidParam);
}

std::string CAENDigitizerBoard::GetEventPackaging()
{
  CAEN_DGTZ_EnaDis_t enable;
  CAENDigitizerError(CAEN_DGTZ_GetEventPackaging(m_Handle,&enable));
  if(enable==CAEN_DGTZ_ENABLE) return "ENABLE";
  else return "DISABLE";
}

void CAENDigitizerBoard::SetMaxNumAggregatesBLT(const std::uint32_t& numAggr)
{
  CAENDigitizerError(CAEN_DGTZ_SetMaxNumAggregatesBLT(m_Handle,numAggr));
}

void CAENDigitizerBoard::SetMaxNumEventsBLT(const std::uint32_t& numEvents)
{
  CAENDigitizerError(CAEN_DGTZ_SetMaxNumEventsBLT(m_Handle,numEvents));
}

std::uint32_t CAENDigitizerBoard::GetMaxNumAggregatesBLT()
{
  std::uint32_t numAggr{0};
  CAENDigitizerError(CAEN_DGTZ_GetMaxNumAggregatesBLT(m_Handle,&numAggr));
  return numAggr;
}

std::uint32_t CAENDigitizerBoard::GetMaxNumEventsBLT()
{
  std::uint32_t numEvents{0};
  CAENDigitizerError(CAEN_DGTZ_GetMaxNumEventsBLT(m_Handle,&numEvents));
  return numEvents;
}

std::uint32_t CAENDigitizerBoard::GetNumEvents()
{
  std::uint32_t numEvents{0};
  CAENDigitizerError(CAEN_DGTZ_GetNumEvents(m_Handle,m_Buffer.get(),m_BufferSize,&numEvents));
  return numEvents;
}

std::uint32_t CAENDigitizerBoard::GetChannelGroupMask(const std::uint32_t& group)
{
  std::uint32_t channelmask{0};
  CAENDigitizerError(CAEN_DGTZ_GetChannelGroupMask(m_Handle,group,&channelmask));
  return  channelmask;
}

void CAENDigitizerBoard::SetDPPPreTriggerSize(const int& ch,const std::uint32_t& samples)
{
  CAENDigitizerError(CAEN_DGTZ_SetDPPPreTriggerSize(m_Handle,ch,samples));
}

std::uint32_t CAENDigitizerBoard::GetDPPPreTriggerSize(const int& ch)
{
  std::uint32_t samples{0};
  CAEN_DGTZ_GetDPPPreTriggerSize(m_Handle,ch,&samples);
  return samples;
}


}
