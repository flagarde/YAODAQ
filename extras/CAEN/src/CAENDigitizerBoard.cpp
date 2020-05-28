#include "CAENDigitizerType.h"
#include "CAENDigitizerBoard.hpp"
#include "Utilities.hpp"


#include "CAENDigitizer.h"

#include "CAENDigitizerException.hpp"
#include "Flash.hpp"
#include <iostream>





namespace CAEN
{

  Json::Value parse(const CAEN_DGTZ_X742_EVENT_t* p)
  {
    Json::Value ret;
    Json::Value groups = Json::Value(Json::arrayValue);
    for(std::size_t i=0;i!=MAX_X742_GROUP_SIZE;++i)
    {
      if(p->GrPresent[i]==1)
      {
        Json::Value group;
        group["Groupe"]=i;
        group["TriggerTimeTag"]=p->DataGroup[i].TriggerTimeTag;
        group["StartIndexCell"]=p->DataGroup[i].StartIndexCell;
        Json::Value channels = Json::Value(Json::arrayValue);
        for(std::size_t j=0;j!=MAX_X742_CHANNEL_SIZE;++j)
        {
          Json::Value channel;
          if(p->DataGroup[i].ChSize[j]!=0)
          {
            channel["Channel"]=j;
            Json::Value data=Json::Value(Json::arrayValue);
            for(std::size_t jj=0;jj!=p->DataGroup[i].ChSize[j];++jj)
            {
              data.append(p->DataGroup[i].DataChannel[j][jj]);
            }
            channel["Data"]=data;
            channels.append(channel);
          }
        }
        group["Channels"]=channels;
        groups.append(group);
      }
    }
    ret["Groups"]=groups;
    return ret;
  }

  /*******************DPPAcquisitionMode**************************/
DPPAcquisitionMode::DPPAcquisitionMode(const std::string& mode,const std::string& param)
{
  setAcqMode(mode);
  setSaveParam(param);
}
  
void DPPAcquisitionMode::setAcqMode(const std::string& mode)
{
  if(mode == "Oscilloscope" || mode == "List" || mode == "Mixed") AcqMode = mode;
  else throw CAENDigitizerException(CAEN_DGTZ_InvalidParam);
}
  
void DPPAcquisitionMode::setSaveParam(const std::string& param)
{
  if(param == "EnergyOnly" || param == "TimeOnly" || param == "EnergyAndTime" || param == "None") SaveParam = param;
  else throw CAENDigitizerException(CAEN_DGTZ_InvalidParam);
}
  
std::string DPPAcquisitionMode::getAcqMode()
{
  return AcqMode;
}
  
std::string DPPAcquisitionMode::getSaveParam()
{
  return SaveParam;
}
  
/*******************InterruptConfig**************************/
std::string InterruptConfig::getState()const{return m_State;}
std::uint8_t InterruptConfig::getLevel()const{return m_Level;}
std::uint32_t InterruptConfig::getStatusID()const{return m_StatusID;}
std::string InterruptConfig::getMode()const{return m_Mode;}
std::uint16_t InterruptConfig::getEventNumber()const{return m_EventNumber;}
void InterruptConfig::setState(const std::string& state){m_State=state;}
void InterruptConfig::setLevel(const std::uint8_t& level){m_Level=level;}
void InterruptConfig::setStatusID(const std::uint32_t statusID){m_StatusID=statusID;}
void InterruptConfig::setMode(const std::string& mode){m_Mode=mode;}
void InterruptConfig::setEventNumber(const std::uint32_t eventNumber){m_EventNumber=eventNumber;}  
  
  
  
class EventInfo
{
public:
  EventInfo(const CAEN_DGTZ_EventInfo_t& eventInfos, char* event) { t = eventInfos; m_Event=event; }
  std::uint32_t getEventSize() { return t.EventSize; }
  std::uint32_t getBoardId() { return t.BoardId; }
  std::uint32_t getPattern() { return t.Pattern; }
  std::uint32_t getChannelMask() { return t.ChannelMask; }
  std::uint32_t getEventCounter() { return t.EventCounter; }
  std::uint32_t getTriggerTimeTag() { return t.TriggerTimeTag; }
  char* getEvent() {return m_Event;}
private:
  CAEN_DGTZ_EventInfo_t t;
  char* m_Event{nullptr};
};

CAENDigitizerBoard::CAENDigitizerBoard(const std::string& name):Board(name, "CAENDigitizerBoard")
{
  initilizeParameters();
}

///////////////////////////////////////////////////////////////////////////
/////////////////FROM CAENDIGITIZER LIBRARY

void CAENDigitizerBoard::WriteRegister(const std::uint32_t& Address, const std::uint32_t& Data)
{
  CAENDigitizerException(CAEN_DGTZ_WriteRegister(m_Handle, Address, Data));
}

std::uint32_t CAENDigitizerBoard::ReadRegister(const std::uint32_t& Address)
{
  std::uint32_t ret{0};
  CAENDigitizerException(CAEN_DGTZ_ReadRegister(m_Handle, Address, &ret));
  return ret;
}

void CAENDigitizerBoard::GetInfo()
{
  CAEN_DGTZ_BoardInfo_t boardinfo;
  CAENDigitizerException(CAEN_DGTZ_GetInfo(m_Handle, &boardinfo));
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
  setHasDPPFirware();
}

void CAENDigitizerBoard::Reset()
{
  CAENDigitizerException(CAEN_DGTZ_Reset(m_Handle));
}

void CAENDigitizerBoard::ClearData()
{
  CAENDigitizerException(CAEN_DGTZ_ClearData(m_Handle));
}

void CAENDigitizerBoard::SendSWtrigger()
{
  CAENDigitizerException(CAEN_DGTZ_SendSWtrigger(m_Handle));
}

void CAENDigitizerBoard::SWStartAcquisition()
{
  CAENDigitizerException(CAEN_DGTZ_SWStartAcquisition(m_Handle));
}

void CAENDigitizerBoard::SWStopAcquisition()
{
  CAENDigitizerException(CAEN_DGTZ_SWStopAcquisition(m_Handle));
}

void CAENDigitizerBoard::SetInterruptConfig(const InterruptConfig& interruptConfig)
{
  CAEN_DGTZ_EnaDis_t state;
  if(interruptConfig.getState() == "ENABLED") state = CAEN_DGTZ_ENABLE;
  else if(interruptConfig.getState() == "DISABLED")state = CAEN_DGTZ_DISABLE;
  else throw CAENDigitizerException(CAEN_DGTZ_InvalidParam);
  CAEN_DGTZ_IRQMode_t mode;
  if(interruptConfig.getMode() == "RORA") mode = CAEN_DGTZ_IRQ_MODE_RORA;
  else if(interruptConfig.getMode() == "ROAK")mode = CAEN_DGTZ_IRQ_MODE_ROAK;
  else throw CAENDigitizerException(CAEN_DGTZ_InvalidParam);
  CAENDigitizerException(CAEN_DGTZ_SetInterruptConfig(m_Handle,state,interruptConfig.getLevel(), interruptConfig.getStatusID(),interruptConfig.getEventNumber(), mode));
}

InterruptConfig CAENDigitizerBoard::GetInterruptConfig()
{
  CAEN_DGTZ_IRQMode_t mode;
  std::uint16_t eventNumber{0};
  std::uint32_t statusID{0};
  std::uint8_t level{0};
  CAEN_DGTZ_EnaDis_t state;
  CAENDigitizerException(CAEN_DGTZ_GetInterruptConfig(m_Handle,&state,&level,&statusID,&eventNumber,&mode));
  InterruptConfig ret;
  if(state==CAEN_DGTZ_ENABLE) ret.setState("ENABLED");
  else ret.setState("DISABLED");
  if(mode==CAEN_DGTZ_IRQ_MODE_RORA) ret.setMode("RORA");
  else ret.setMode("ROAK");
  ret.setLevel(level);
  ret.setStatusID(statusID);
  ret.setEventNumber(eventNumber);
  return ret;
}

void CAENDigitizerBoard::IRQWait(const std::uint32_t& timeout)
{
  CAENDigitizerException(CAEN_DGTZ_IRQWait(m_Handle, timeout));
}

uint32_t CAENDigitizerBoard::VMEIRQWait(const std::string name,const uint8_t& IRQMask,const uint32_t& timeout)
{
  toml::value config = m_Config.getConnectorInfos(name).getParameters();
  std::string m_ConnectionType=toml::find_or<std::string>(config, "Connection Type","");
  int m_LinkNum = toml::find<int>(config, "Link Number");
  int m_ConetNode{0};
  if(m_ConnectionType == "OPTICAL")
  {
    m_ConetNode = toml::find<int>(config, "Conet Node");
  }
  int handle{0};
  std::cout<<m_ConnectionType<<"  "<<m_LinkNum<<"  "<<m_ConetNode<<std::endl;
  CAEN_DGTZ_VMEIRQWait((CAEN_DGTZ_ConnectionType)CAEN_DGTZ_USB,m_LinkNum, m_ConetNode,IRQMask,timeout,&handle);
 /* if(m_ConnectionType=="USB")CAENDigitizerException(CAEN_DGTZ_VMEIRQWait(CAEN_DGTZ_USB,m_LinkNum, m_ConetNode,IRQMask,timeout,&handle));
  else CAENDigitizerException(CAEN_DGTZ_VMEIRQWait(CAEN_DGTZ_OpticalLink,m_LinkNum, m_ConetNode,IRQMask,timeout,&handle));*/
  return handle;
}


/**************************************************************************/ /**
* \brief     Checks VME interrupt level
*
* \param   [IN] VMEhandle : handle of CAEN VME Bridge that raised the interrupt request (retrieved from CAEN_DGTZ_VMEIRQWait function)
* \param   [OUT] Mask     : bitmask representing the active VME interrupt requests.
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
// CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_VMEIRQCheck(int VMEHandle,
// uint8_t *Mask);

int32_t CAENDigitizerBoard::VMEIACKCycle(const int& VMEHandle,const uint8_t& level)
{
  int32_t boardid{0};
  CAENDigitizerException(CAEN_DGTZ_VMEIACKCycle(VMEHandle,level,&boardid));
  return boardid;
}

void CAENDigitizerBoard::SetDESMode(const std::string& on)
{
  if(on == "ENABLED") CAENDigitizerException(CAEN_DGTZ_SetDESMode(m_Handle, CAEN_DGTZ_ENABLE));
  else if(on == "DISABLED") CAENDigitizerException(CAEN_DGTZ_SetDESMode(m_Handle, CAEN_DGTZ_DISABLE));
  else throw CAENDigitizerException(CAEN_DGTZ_InvalidParam);
}

std::string CAENDigitizerBoard::GetDESMode()
{
  CAEN_DGTZ_EnaDis_t enable;
  CAENDigitizerException(CAEN_DGTZ_GetDESMode(m_Handle, &enable));
  if(enable == CAEN_DGTZ_ENABLE) return "ENABLED";
  else return "DISABLED";
}

void CAENDigitizerBoard::SetRecordLength(const std::uint32_t& size, const int& ch)
{
  if(ch==-1)CAENDigitizerException(CAEN_DGTZ_SetRecordLength(m_Handle,size));
  else CAENDigitizerException(CAEN_DGTZ_SetRecordLength(m_Handle, size, ch));
}

std::uint32_t CAENDigitizerBoard::GetRecordLength(const int& ch)
{
  std::uint32_t size{0};
  if(ch == -1) CAENDigitizerException(CAEN_DGTZ_GetRecordLength(m_Handle, &size));
  else CAENDigitizerException(CAEN_DGTZ_GetRecordLength(m_Handle, &size, ch));
  return size;
}

void CAENDigitizerBoard::SetChannelEnableMask(const std::uint32_t& mask)
{
  CAENDigitizerException(CAEN_DGTZ_SetChannelEnableMask(m_Handle, mask));
}

std::uint32_t CAENDigitizerBoard::GetChannelEnableMask()
{
  std::uint32_t ret{0};
  CAENDigitizerException(CAEN_DGTZ_GetChannelEnableMask(m_Handle, &ret));
  return ret;
}

void CAENDigitizerBoard::SetGroupEnableMask(const std::uint32_t& mask)
{
  CAENDigitizerException(CAEN_DGTZ_SetGroupEnableMask(m_Handle, mask));
}

std::uint32_t CAENDigitizerBoard::GetGroupEnableMask()
{
  std::uint32_t ret{0};
  CAENDigitizerException(CAEN_DGTZ_GetGroupEnableMask(m_Handle, &ret));
  return ret;
}

void CAENDigitizerBoard::SetSWTriggerMode(const std::string& mode)
{
  CAEN_DGTZ_TriggerMode_t t;
  if(mode == "DISABLED") t = CAEN_DGTZ_TRGMODE_DISABLED;
  else if(mode == "EXTOUT_ONLY") t = CAEN_DGTZ_TRGMODE_EXTOUT_ONLY;
  else if(mode == "ACQ_ONLY") t = CAEN_DGTZ_TRGMODE_ACQ_ONLY;
  else if(mode == "ACQ_AND_EXTOUT") t = CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT;
  else throw CAENDigitizerException(CAEN_DGTZ_InvalidParam);
  CAENDigitizerException(CAEN_DGTZ_SetSWTriggerMode(m_Handle, t));
}

void CAENDigitizerBoard::SetExtTriggerInputMode(const std::string& mode)
{
  CAEN_DGTZ_TriggerMode_t t;
  if(mode == "DISABLED") t = CAEN_DGTZ_TRGMODE_DISABLED;
  else if(mode == "EXTOUT_ONLY") t = CAEN_DGTZ_TRGMODE_EXTOUT_ONLY;
  else if(mode == "ACQ_ONLY") t = CAEN_DGTZ_TRGMODE_ACQ_ONLY;
  else if(mode == "ACQ_AND_EXTOUT") t = CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT;
  else throw CAENDigitizerException(CAEN_DGTZ_InvalidParam);
  CAENDigitizerException(CAEN_DGTZ_SetExtTriggerInputMode(m_Handle, t));
}

std::string CAENDigitizerBoard::GetExtTriggerInputMode()
{
  CAEN_DGTZ_TriggerMode_t t;
  CAENDigitizerException(CAEN_DGTZ_GetExtTriggerInputMode(m_Handle, &t));
  if(t == CAEN_DGTZ_TRGMODE_DISABLED) return "DISABLED";
  else if(t == CAEN_DGTZ_TRGMODE_EXTOUT_ONLY) return "EXTOUT_ONLY";
  else if(t == CAEN_DGTZ_TRGMODE_ACQ_ONLY) return "ACQ_ONLY";
  else return "ACQ_AND_EXTOUT";
}

std::string CAENDigitizerBoard::GetSWTriggerMode()
{
  CAEN_DGTZ_TriggerMode_t t;
  CAENDigitizerException(CAEN_DGTZ_GetSWTriggerMode(m_Handle, &t));
  if(t == CAEN_DGTZ_TRGMODE_DISABLED) return "DISABLED";
  else if(t == CAEN_DGTZ_TRGMODE_EXTOUT_ONLY) return "EXTOUT_ONLY";
  else if(t == CAEN_DGTZ_TRGMODE_ACQ_ONLY) return "ACQ_ONLY";
  else return "ACQ_AND_EXTOUT";
}

void CAENDigitizerBoard::SetChannelSelfTrigger(const std::string&   mode, const std::uint32_t& channelmask)
{
  CAEN_DGTZ_TriggerMode_t t;
  if(mode == "DISABLED") t = CAEN_DGTZ_TRGMODE_DISABLED;
  else if(mode == "EXTOUT_ONLY") t = CAEN_DGTZ_TRGMODE_EXTOUT_ONLY;
  else if(mode == "ACQ_ONLY") t = CAEN_DGTZ_TRGMODE_ACQ_ONLY;
  else if(mode == "ACQ_AND_EXTOUT") t = CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT;
  else throw CAENDigitizerException(CAEN_DGTZ_InvalidParam);
  CAENDigitizerException(CAEN_DGTZ_SetChannelSelfTrigger(m_Handle, t, channelmask));
}

std::string CAENDigitizerBoard::GetChannelSelfTrigger(const std::uint32_t& channel)
{
  CAEN_DGTZ_TriggerMode_t t;
  CAENDigitizerException(CAEN_DGTZ_GetChannelSelfTrigger(m_Handle, channel, &t));
  if(t == CAEN_DGTZ_TRGMODE_DISABLED) return "DISABLED";
  else if(t == CAEN_DGTZ_TRGMODE_EXTOUT_ONLY) return "EXTOUT_ONLY";
  else if(t == CAEN_DGTZ_TRGMODE_ACQ_ONLY) return "ACQ_ONLY";
  else return "ACQ_AND_EXTOUT";
}

void CAENDigitizerBoard::SetGroupSelfTrigger(const std::string&   mode, const std::uint32_t& group)
{
  CAEN_DGTZ_TriggerMode_t t;
  if(mode == "DISABLED") t = CAEN_DGTZ_TRGMODE_DISABLED;
  else if(mode == "EXTOUT_ONLY") t = CAEN_DGTZ_TRGMODE_EXTOUT_ONLY;
  else if(mode == "ACQ_ONLY") t = CAEN_DGTZ_TRGMODE_ACQ_ONLY;
  else if(mode == "ACQ_AND_EXTOUT") t = CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT;
  else throw CAENDigitizerException(CAEN_DGTZ_InvalidParam);
  CAENDigitizerException(CAEN_DGTZ_SetGroupSelfTrigger(m_Handle, t, group));
}

std::string CAENDigitizerBoard::GetGroupSelfTrigger(const std::uint32_t& group)
{
  CAEN_DGTZ_TriggerMode_t t;
  CAENDigitizerException(CAEN_DGTZ_GetGroupSelfTrigger(m_Handle, group, &t));
  if(t == CAEN_DGTZ_TRGMODE_DISABLED) return "DISABLED";
  else if(t == CAEN_DGTZ_TRGMODE_EXTOUT_ONLY) return "EXTOUT_ONLY";
  else if(t == CAEN_DGTZ_TRGMODE_ACQ_ONLY) return "ACQ_ONLY";
  else return "ACQ_AND_EXTOUT";
}

void CAENDigitizerBoard::SetChannelGroupMask(const std::uint32_t  group, const std::uint32_t& channelmask)
{
  CAENDigitizerException(CAEN_DGTZ_SetChannelGroupMask(m_Handle, group, channelmask));
}

std::uint32_t CAENDigitizerBoard::GetChannelGroupMask(const std::uint32_t& group)
{
  std::uint32_t channelmask{0};
  CAENDigitizerException(CAEN_DGTZ_GetChannelGroupMask(m_Handle, group, &channelmask));
  return channelmask;
}

void CAENDigitizerBoard::SetPostTriggerSize(const std::uint32_t& percent)
{
  CAENDigitizerException(CAEN_DGTZ_SetPostTriggerSize(m_Handle, percent));
}

std::uint32_t CAENDigitizerBoard::GetPostTriggerSize()
{
  std::uint32_t percent{0};
  CAENDigitizerException(CAEN_DGTZ_GetPostTriggerSize(m_Handle, &percent));
  return percent;
}

void CAENDigitizerBoard::SetDPPPreTriggerSize(const int& ch, const std::uint32_t& samples)
{
  CAENDigitizerException(CAEN_DGTZ_SetDPPPreTriggerSize(m_Handle, ch, samples));
}

std::uint32_t CAENDigitizerBoard::GetDPPPreTriggerSize(const int& ch)
{
  std::uint32_t samples{0};
  CAENDigitizerException(CAEN_DGTZ_GetDPPPreTriggerSize(m_Handle, ch, &samples));
  return samples;
}

void CAENDigitizerBoard::SetChannelDCOffset(const std::uint32_t& channel, const std::uint32_t& Tvalue)
{
  CAENDigitizerException(CAEN_DGTZ_SetChannelDCOffset(m_Handle, channel, Tvalue));
}

std::uint32_t CAENDigitizerBoard::GetChannelDCOffset(const std::uint32_t& channel)
{
  std::uint32_t Tvalue{0};
  CAENDigitizerException(CAEN_DGTZ_GetChannelDCOffset(m_Handle, channel, &Tvalue));
  return Tvalue;
}

void CAENDigitizerBoard::SetGroupDCOffset(const std::uint32_t& group, const std::uint32_t& Tvalue)
{
  CAENDigitizerException(CAEN_DGTZ_SetGroupDCOffset(m_Handle, group, Tvalue));
}

std::uint32_t CAENDigitizerBoard::GetGroupDCOffset(const std::uint32_t& group)
{
  std::uint32_t Tvalue{0};
  CAENDigitizerException(CAEN_DGTZ_GetGroupDCOffset(m_Handle, group, &Tvalue));
  return Tvalue;
}

void CAENDigitizerBoard::SetChannelTriggerThreshold(const std::uint32_t& channel, const std::uint32_t& Tvalue)
{
  CAENDigitizerException(CAEN_DGTZ_SetChannelTriggerThreshold(m_Handle, channel, Tvalue));
}

std::uint32_t CAENDigitizerBoard::GetChannelTriggerThreshold(const std::uint32_t& channel)
{
  std::uint32_t Tvalue{0};
  CAENDigitizerException(CAEN_DGTZ_GetChannelTriggerThreshold(m_Handle, channel, &Tvalue));
  return Tvalue;
}

void CAENDigitizerBoard::SetChannelPulsePolarity(const std::uint32_t& channel, const std::string&   pol)
{
  if(pol == "POSITIVE") CAENDigitizerException(CAEN_DGTZ_SetChannelPulsePolarity(m_Handle, channel, CAEN_DGTZ_PulsePolarityPositive));
  else if(pol == "NEGATIVE") CAENDigitizerException(CAEN_DGTZ_SetChannelPulsePolarity(m_Handle, channel, CAEN_DGTZ_PulsePolarityNegative));
  else throw CAENDigitizerException(CAEN_DGTZ_InvalidParam);
}

std::string CAENDigitizerBoard::GetChannelPulsePolarity(const std::uint32_t& channel)
{
  CAEN_DGTZ_PulsePolarity_t pol;
  CAENDigitizerException(CAEN_DGTZ_GetChannelPulsePolarity(m_Handle, channel, &pol));
  if(pol == CAEN_DGTZ_PulsePolarityPositive) return "POSITIVE";
  else return "NEGATIVE";
}

void CAENDigitizerBoard::SetGroupTriggerThreshold(const std::uint32_t& group, const std::uint32_t& Tvalue)
{
  CAENDigitizerException(CAEN_DGTZ_SetGroupTriggerThreshold(m_Handle, group, Tvalue));
}

std::uint32_t CAENDigitizerBoard::GetGroupTriggerThreshold(const std::uint32_t& group)
{
  std::uint32_t Tvalue{0};
  CAENDigitizerException(CAEN_DGTZ_GetGroupTriggerThreshold(m_Handle, group, &Tvalue));
  return Tvalue;
}

void CAENDigitizerBoard::SetZeroSuppressionMode(const std::string& mode)
{
  if(mode == "NO") CAENDigitizerException(CAEN_DGTZ_SetZeroSuppressionMode(m_Handle, CAEN_DGTZ_ZS_NO));
  else if(mode == "INT") CAENDigitizerException(CAEN_DGTZ_SetZeroSuppressionMode(m_Handle, CAEN_DGTZ_ZS_INT));
  else if(mode == "ZLE") CAENDigitizerException( CAEN_DGTZ_SetZeroSuppressionMode(m_Handle, CAEN_DGTZ_ZS_ZLE));
  else if(mode == "AMP") CAENDigitizerException( CAEN_DGTZ_SetZeroSuppressionMode(m_Handle, CAEN_DGTZ_ZS_AMP));
  else throw CAENDigitizerException(CAEN_DGTZ_InvalidParam);
}

std::string CAENDigitizerBoard::GetZeroSuppressionMode()
{
  CAEN_DGTZ_ZS_Mode_t mode;
  CAENDigitizerException(CAEN_DGTZ_GetZeroSuppressionMode(m_Handle, &mode));
  switch(mode)
  {
    case CAEN_DGTZ_ZS_NO: return "NO";
    case CAEN_DGTZ_ZS_INT: return "INT";
    case CAEN_DGTZ_ZS_ZLE: return "ZLE";
    default: return "AMP";
  }
}

void CAENDigitizerBoard::SetAcquisitionMode(const std::string& mode)
{
  if(mode == "SW_CONTROLLED") CAENDigitizerException(CAEN_DGTZ_SetAcquisitionMode(m_Handle, CAEN_DGTZ_SW_CONTROLLED));
  else if(mode == "IN_CONTROLLED") CAENDigitizerException( CAEN_DGTZ_SetAcquisitionMode(m_Handle, CAEN_DGTZ_S_IN_CONTROLLED));
  else if(mode == "FIRST_TRG_CONTROLLED") CAENDigitizerException( CAEN_DGTZ_SetAcquisitionMode(m_Handle, CAEN_DGTZ_FIRST_TRG_CONTROLLED));
  else if(mode == "LVDS_CONTROLLED") CAENDigitizerException( CAEN_DGTZ_SetAcquisitionMode(m_Handle, CAEN_DGTZ_LVDS_CONTROLLED));
  else throw CAENDigitizerException(CAEN_DGTZ_InvalidParam);
}

std::string CAENDigitizerBoard::GetAcquisitionMode()
{
  CAEN_DGTZ_AcqMode_t mode;
  CAENDigitizerException(CAEN_DGTZ_GetAcquisitionMode(m_Handle, &mode));
  switch(mode)
  {
    case CAEN_DGTZ_SW_CONTROLLED: return "SW_CONTROLLED";
    case CAEN_DGTZ_S_IN_CONTROLLED: return "S_IN_CONTROLLED";
    case CAEN_DGTZ_FIRST_TRG_CONTROLLED: return "FIRST_TRG_CONTROLLED";
    default: return "LVDS_CONTROLLED";
  }
}

void CAENDigitizerBoard::SetRunSynchronizationMode(const std::string& mode)
{
  if(mode == "Disabled") CAENDigitizerException(CAEN_DGTZ_SetRunSynchronizationMode(m_Handle, CAEN_DGTZ_RUN_SYNC_Disabled));
  else if(mode == "TrgOutTrgInDaisyChain") CAENDigitizerException(CAEN_DGTZ_SetRunSynchronizationMode(m_Handle, CAEN_DGTZ_RUN_SYNC_TrgOutTrgInDaisyChain));
  else if(mode == "TrgOutSinDaisyChain") CAENDigitizerException(CAEN_DGTZ_SetRunSynchronizationMode(m_Handle, CAEN_DGTZ_RUN_SYNC_TrgOutSinDaisyChain));
  else if(mode == "SinFanout") CAENDigitizerException(CAEN_DGTZ_SetRunSynchronizationMode(m_Handle, CAEN_DGTZ_RUN_SYNC_SinFanout));
  else if(mode == "CAEN_DGTZ_RUN_SYNC_GpioGpioDaisyChain") CAENDigitizerException(CAEN_DGTZ_SetRunSynchronizationMode(m_Handle, CAEN_DGTZ_RUN_SYNC_GpioGpioDaisyChain));
  else throw CAENDigitizerException(CAEN_DGTZ_InvalidParam);
}

std::string CAENDigitizerBoard::GetRunSynchronizationMode()
{
  CAEN_DGTZ_RunSyncMode_t mode;
  CAENDigitizerException(CAEN_DGTZ_GetRunSynchronizationMode(m_Handle, &mode));
  switch(mode)
  {
    case CAEN_DGTZ_RUN_SYNC_Disabled: return "Disabled";
    case CAEN_DGTZ_RUN_SYNC_TrgOutTrgInDaisyChain: return "TrgOutTrgInDaisyChain";
    case CAEN_DGTZ_RUN_SYNC_TrgOutSinDaisyChain: return "TrgOutSinDaisyChain";
    case CAEN_DGTZ_RUN_SYNC_SinFanout: return "SinFanout";
    default: return "GpioGpioDaisyChain";
  }
}

void CAENDigitizerBoard::SetAnalogMonOutput(const std::string& mode)
{
  CAEN_DGTZ_AnalogMonitorOutputMode_t t;
  if(mode == "TRIGGER_MAJORITY") t = CAEN_DGTZ_AM_TRIGGER_MAJORITY;
  else if(mode == "TEST") t = CAEN_DGTZ_AM_TEST;
  else if(mode == "ANALOG_INSPECTION") t = CAEN_DGTZ_AM_ANALOG_INSPECTION;
  else if(mode == "BUFFER_OCCUPANCY") t = CAEN_DGTZ_AM_BUFFER_OCCUPANCY;
  else if(mode == "VOLTAGE_LEVEL") t = CAEN_DGTZ_AM_VOLTAGE_LEVEL;
  else throw CAENDigitizerException(CAEN_DGTZ_InvalidParam);
  CAENDigitizerException(CAEN_DGTZ_SetAnalogMonOutput(m_Handle, t));
}

std::string CAENDigitizerBoard::GetAnalogMonOutput()
{
  CAEN_DGTZ_AnalogMonitorOutputMode_t t;
  CAENDigitizerException(CAEN_DGTZ_GetAnalogMonOutput(m_Handle, &t));
  if(t == CAEN_DGTZ_AM_TRIGGER_MAJORITY) return "TRIGGER_MAJORITY";
  else if(t == CAEN_DGTZ_AM_TEST) return "TEST";
  else if(t == CAEN_DGTZ_AM_ANALOG_INSPECTION) return "ANALOG_INSPECTION";
  else if(t == CAEN_DGTZ_AM_BUFFER_OCCUPANCY) return "BUFFER_OCCUPANCY";
  else return "VOLTAGE_LEVEL";
}

/**************************************************************************/ /**
* \fn          CAEN_DGTZ_SetAnalogInspectionMonParams(int handle, uint32_t channelmask, uint32_t offset, CAEN_DGTZ_AnalogMonitorMagnify_t mf, CAEN_DGTZ_AnalogMonitorInspectorInverter_t ami)
* \brief     Sets the Analog Inspection Monitor parameters for a digitizer
* \param     [IN] channelmask : channel enable mask for Analog Inspection
* \param     [IN] offset      : Analog Inspection Offset
* \param     [IN] mf          : Analog Inspection Multiply Factor
* \param     [IN] ami         : Analog Inspection Invert
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
// CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetAnalogInspectionMonParams(int
// handle, uint32_t channelmask, uint32_t offset,
// CAEN_DGTZ_AnalogMonitorMagnify_t mf,
// CAEN_DGTZ_AnalogMonitorInspectorInverter_t ami);

/**************************************************************************/ /**
* \brief     Gets Analog Inspection Monitor parameters from a digitizer
* \param     [OUT] channelmask : channel enable mask for Analog Inspection
* \param     [OUT] offset      : Analog Inspection Offset
* \param     [OUT] mf          : Analog Inspection Multiply Factor
* \param     [OUT] ami         : Analog Inspection Invert
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
// CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetAnalogInspectionMonParams(int
// handle, uint32_t *channelmask, uint32_t *offset,
// CAEN_DGTZ_AnalogMonitorMagnify_t *mf,
// CAEN_DGTZ_AnalogMonitorInspectorInverter_t *ami);

void CAENDigitizerBoard::DisableEventAlignedReadout()
{
  CAENDigitizerException(CAEN_DGTZ_DisableEventAlignedReadout(m_Handle));
}

void CAENDigitizerBoard::SetEventPackaging(const std::string& on)
{
  if(on == "ENABLED") CAENDigitizerException(CAEN_DGTZ_SetEventPackaging(m_Handle, CAEN_DGTZ_ENABLE));
  else if(on == "DISABLED") CAENDigitizerException( CAEN_DGTZ_SetEventPackaging(m_Handle, CAEN_DGTZ_DISABLE));
  else throw CAENDigitizerException(CAEN_DGTZ_InvalidParam);
}

std::string CAENDigitizerBoard::GetEventPackaging()
{
  CAEN_DGTZ_EnaDis_t enable;
  CAENDigitizerException(CAEN_DGTZ_GetEventPackaging(m_Handle, &enable));
  if(enable == CAEN_DGTZ_ENABLE) return "ENABLED";
  else return "DISABLED";
}

void CAENDigitizerBoard::SetMaxNumAggregatesBLT(const std::uint32_t& numAggr)
{
  CAENDigitizerException(CAEN_DGTZ_SetMaxNumAggregatesBLT(m_Handle, numAggr));
}

void CAENDigitizerBoard::SetMaxNumEventsBLT(const std::uint32_t& numEvents)
{
  CAENDigitizerException(CAEN_DGTZ_SetMaxNumEventsBLT(m_Handle, numEvents));
}

std::uint32_t CAENDigitizerBoard::GetMaxNumAggregatesBLT()
{
  std::uint32_t numAggr{0};
  CAENDigitizerException(CAEN_DGTZ_GetMaxNumAggregatesBLT(m_Handle, &numAggr));
  return numAggr;
}

std::uint32_t CAENDigitizerBoard::GetMaxNumEventsBLT()
{
  std::uint32_t numEvents{0};
  CAENDigitizerException(CAEN_DGTZ_GetMaxNumEventsBLT(m_Handle, &numEvents));
  return numEvents;
}

void CAENDigitizerBoard::MallocReadoutBuffer()
{
  if(m_Buffer==nullptr) CAENDigitizerException(CAEN_DGTZ_MallocReadoutBuffer(m_Handle, &m_Buffer, &m_AllocatedSize));
}

void CAENDigitizerBoard::ReadData(const std::string& mode)
{
  CAEN_DGTZ_ReadData(m_Handle,CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT,m_Buffer, &m_BufferSize);
  /*if(mode == "SLAVE_TERMINATED_READOUT_MBLT") CAENDigitizerException( CAEN_DGTZ_ReadData(m_Handle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT,m_Buffer, &m_BufferSize));
  else if(mode == "SLAVE_TERMINATED_READOUT_2eVME") CAENDigitizerException(CAEN_DGTZ_ReadData(m_Handle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_2eVME,m_Buffer, &m_BufferSize));
  else if(mode == "SLAVE_TERMINATED_READOUT_2eSST") CAENDigitizerException( CAEN_DGTZ_ReadData(m_Handle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_2eSST,m_Buffer, &m_BufferSize));
  else if(mode == "POLLING_MBLT") CAENDigitizerException(CAEN_DGTZ_ReadData(m_Handle, CAEN_DGTZ_POLLING_MBLT,m_Buffer, &m_BufferSize));
  else if(mode == "POLLING_2eVME") CAENDigitizerException(CAEN_DGTZ_ReadData(m_Handle, CAEN_DGTZ_POLLING_2eVME,m_Buffer, &m_BufferSize));
  else if(mode == "POLLING_2eSST") CAENDigitizerException(CAEN_DGTZ_ReadData(m_Handle, CAEN_DGTZ_POLLING_2eSST,m_Buffer, &m_BufferSize));
  else CAENDigitizerException((int)CAEN_DGTZ_InvalidParam);*/
}

void CAENDigitizerBoard::FreeReadoutBuffer()
{
  if(m_Buffer!=nullptr)
  {
    CAENDigitizerException(CAEN_DGTZ_FreeReadoutBuffer(&m_Buffer));
    m_Buffer=nullptr;
  }
}

std::uint32_t CAENDigitizerBoard::GetNumEvents()
{
  std::uint32_t numEvents{0};
  CAENDigitizerException(CAEN_DGTZ_GetNumEvents(m_Handle, m_Buffer,m_BufferSize, &numEvents));
  return numEvents;
}

EventInfo CAENDigitizerBoard::GetEventInfo(const std::int32_t& eventnumb)
{
  CAEN_DGTZ_EventInfo_t info;
  char* event{nullptr};
  CAENDigitizerException(CAEN_DGTZ_GetEventInfo(m_Handle,m_Buffer,m_BufferSize,eventnumb,&info,&event));
  EventInfo eventInfo(info,event);
  std::cout<<eventInfo.getTriggerTimeTag()<<std::endl;
  return eventInfo;
}

void CAENDigitizerBoard::DecodeEvent(char* event)
{
  if(GetDPPFirmwareType() == "NODPP")
  {
    CAENDigitizerException(CAEN_DGTZ_DecodeEvent(m_Handle, event,(void**)(&m_Event)));
  }
}

void CAENDigitizerBoard::FreeEvent()
{
  //CAENDigitizerException(CAEN_DGTZ_FreeEvent(m_Handle,(void**)(&m_X742Event)));
}

void CAENDigitizerBoard::MallocDPPEvents()
{
  /*if(GetDPPFirmwareType() == "PHA")
  {
    CAENDigitizerException(CAEN_DGTZ_MallocDPPEvents(m_Handle, (void**)(&m_DPPPHAEvent), &m_AllocatedSizeDPP));
  }
  else if(GetDPPFirmwareType() == "PSD")
  {
    CAENDigitizerException(CAEN_DGTZ_MallocDPPEvents(m_Handle, (void**)(&m_DPPPSDEvent), &m_AllocatedSizeDPP));
  }
  else if(GetDPPFirmwareType() == "CI")
  {
    CAENDigitizerException(CAEN_DGTZ_MallocDPPEvents(m_Handle, (void**)(&m_DPPCIEvent), &m_AllocatedSizeDPP));
  }
  else if(GetDPPFirmwareType() == "QDC")
  {
    CAENDigitizerException(CAEN_DGTZ_MallocDPPEvents(m_Handle, (void**)(&m_DPPQDCEvent), &m_AllocatedSizeDPP));
  }
  else if(GetDPPFirmwareType() == "ZLE" && m_FamilyCode == "XX751")
  {
    CAENDigitizerException(CAEN_DGTZ_MallocDPPEvents(m_Handle, (void**)(&m_751ZLEEvent), &m_AllocatedSizeDPP));
  }
  else if(GetDPPFirmwareType() == "ZLE" && m_FamilyCode == "XX730")
  {
    CAENDigitizerException(CAEN_DGTZ_MallocDPPEvents(m_Handle, (void**)(&m_730ZLEEvent), &m_AllocatedSizeDPP));
  }
  else if(GetDPPFirmwareType() == "DAW" && m_FamilyCode == "XX730")
  {
    CAENDigitizerException(CAEN_DGTZ_MallocDPPEvents(m_Handle, (void**)(&m_730DAWEvent), &m_AllocatedSizeDPP));
  }
  else if(m_FamilyCode == "XX743")
  {
    CAENDigitizerException(CAEN_DGTZ_MallocDPPEvents(m_Handle, (void**)(&m_DPPX743Event), &m_AllocatedSizeDPP));
  }*/
}

//CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_FreeDPPEvents(int handle, void **events); Should be fine with 

/**************************************************************************/ /**
* \brief     Allocates the waveform buffer, which is used by CAEN_DGTZ_DecodeDPPWaveforms.
* \note     Part of the <b>new readout API</b>
* \param     [OUT] waveforms      : pointer to the waveform buffer, which shall be of type:
*                                  - CAEN_DGTZ_DPP_PHA_Waveforms_t, for DPP-PHA
*                                   - CAEN_DGTZ_DPP_PSD_Waveforms_t, for DPP-PSD
*                                   - CAEN_DGTZ_DPP_CI_Waveforms_t, for DPP-CI
* \param    [OUT] allocatedSize  : size in bytes of the waveform buffer
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
// CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_MallocDPPWaveforms(int handle,
// void **waveforms, uint32_t *allocatedSize);

/**************************************************************************/ /**
* \brief     Deallocates the waveform buffer.
* \note     Part of the <b>new readout API</b>
* \param [IN] waveforms  : pointer to the waveform buffer
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
// CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_FreeDPPWaveforms(int handle,
// void *Waveforms);

/**************************************************************************/ /**
* \brief     Decodes the waveforms contained inside an event.
* \note     Part of the <b>new readout API</b>
* \param [IN] events     : pointer to the event
* \param [OUT] waveforms : pointer to the (preallocated) waveform list
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
// CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_DecodeDPPWaveforms(int handle,
// void *event, void *waveforms);

/**************************************************************************/ /**
* \brief     Sets the number of events that each aggregate will contain
* \param [IN] numEvents   : number of events per aggregate
* \param [IN] channel     : optional parameter in the form of an int to
*   specify the channel (required for DPP-PSD and DPP-CI, ignored by DPP-PHA)
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
// CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetNumEventsPerAggregate(int
// handle, uint32_t numEvents, ...);

/**************************************************************************/ /**
* \brief     Gets the number of events that each aggregate will contain
* \deprecated
* \param     [OUT] numEvents  : number of events per aggregate
* \param     [IN] channel     : optional parameter in the form of an int to
*   specify the channel (required for DPP-PSD and DPP-CI, ignored by DPP-PHA)
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
// CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetNumEventsPerAggregate(int
// handle, uint32_t *numEvents, ...);

/**************************************************************************/ /**
* \brief     Set event aggregation parameters
* \note     Only for DPP-PHA, DPP-PSD and DPP-CI
* \param     [IN] threshold   : Specifies how many events to let accumulate in the board memory before they are rendered available for readout.
*                              A low number maximizes responsiveness, since data are read as soon as they are stored in memory, while a high number maximizes efficiency, since fewer transfers are made.
*                              Supplying 0 will let the library choose the best value depending on acquisition mode and other parameters.
* \param     [IN] maxsize     : Specifies the maximum size in bytes of the event buffer on the PC side.
*                              This parameter might be useful in case the computer has very low RAM.
*                              Normally, though, it is safe to supply 0 as this parameter, so that the library will choose an appropriate value automatically.
******************************************************************************/
// CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetDPPEventAggregation(int
// handle, int threshold, int maxsize);

/**************************************************************************/ /**
* \brief     Set DPP configuration parameters for DPP-PHA, DPP-PSD or DPP-CI
* \note     Only for DPP-PHA, DPP-PSD and DPP-CI
* \param     [IN] channelMask : A bit mask indicating the channels to apply the DPP parameters  (not used with x743 family)
* \param     [IN] params      : Pointer to a preallocated struct of type:
*                               - CAEN_DGTZ_DPP_PHA_Params_t, in case of DPP-PHA
*                               - CAEN_DGTZ_DPP_PSD_Params_t,  in case of DPP-PSD
*                               - CAEN_DGTZ_DPP_CI_Params_t, in case of DPP-CI
******************************************************************************/
// CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetDPPParameters(int handle,
// uint32_t channelMask, void* params);

void CAENDigitizerBoard::SetDPPAcquisitionMode(DPPAcquisitionMode& acq)
{
  CAEN_DGTZ_DPP_AcqMode_t   mode;
  CAEN_DGTZ_DPP_SaveParam_t param;
  if(acq.getAcqMode() == "Oscilloscope") mode = CAEN_DGTZ_DPP_ACQ_MODE_Oscilloscope;
  else if(acq.getAcqMode() == "List") mode = CAEN_DGTZ_DPP_ACQ_MODE_List;
  else mode = CAEN_DGTZ_DPP_ACQ_MODE_Mixed;
  if(acq.getSaveParam() == "EnergyOnly") param = CAEN_DGTZ_DPP_SAVE_PARAM_EnergyOnly;
  if(acq.getSaveParam() == "TimeOnly") param = CAEN_DGTZ_DPP_SAVE_PARAM_TimeOnly;
  if(acq.getSaveParam() == "EnergyAndTime") param = CAEN_DGTZ_DPP_SAVE_PARAM_EnergyAndTime;
  else param = CAEN_DGTZ_DPP_SAVE_PARAM_None;
  CAENDigitizerException(CAEN_DGTZ_SetDPPAcquisitionMode(m_Handle, mode, param));
}

DPPAcquisitionMode CAENDigitizerBoard::GetDPPAcquisitionMode()
{
  DPPAcquisitionMode        t;
  CAEN_DGTZ_DPP_AcqMode_t   mode;
  CAEN_DGTZ_DPP_SaveParam_t param;
  CAENDigitizerException(CAEN_DGTZ_GetDPPAcquisitionMode(m_Handle, &mode, &param));
  if(mode == CAEN_DGTZ_DPP_ACQ_MODE_Oscilloscope) t.setAcqMode("Oscilloscope");
  else if(mode == CAEN_DGTZ_DPP_ACQ_MODE_List) t.setAcqMode("List");
  else t.setAcqMode("Mixed");
  if(param == CAEN_DGTZ_DPP_SAVE_PARAM_EnergyOnly) t.setSaveParam("EnergyOnly");
  else if(param == CAEN_DGTZ_DPP_SAVE_PARAM_TimeOnly) t.setSaveParam("TimeOnly");
  else if(param == CAEN_DGTZ_DPP_SAVE_PARAM_EnergyAndTime) t.setSaveParam("EnergyAndTime");
  else t.setSaveParam("None");
  return t;
}

void CAENDigitizerBoard::SetDPPTriggerMode(const std::string& mode)
{
  CAEN_DGTZ_DPP_TriggerMode_t t;
  if(mode == "Normal") t = CAEN_DGTZ_DPP_TriggerMode_Normal;
  else if(mode == "Coincidence") t = CAEN_DGTZ_DPP_TriggerMode_Coincidence;
  else throw CAENDigitizerException(CAEN_DGTZ_InvalidParam);
  CAENDigitizerException(CAEN_DGTZ_GetDPPTriggerMode(m_Handle, &t));
}

std::string CAENDigitizerBoard::GetDPPTriggerMode()
{
  CAEN_DGTZ_DPP_TriggerMode_t t;
  CAENDigitizerException(CAENDGTZ_API CAEN_DGTZ_GetDPPTriggerMode(m_Handle, &t));
  if(t == CAEN_DGTZ_DPP_TriggerMode_Normal) return "Normal";
  else return "Coincidence";
}

void CAENDigitizerBoard::SetDPP_VirtualProbe(const int& trace, const int& probe)
{
  CAENDigitizerException(CAEN_DGTZ_SetDPP_VirtualProbe(m_Handle, trace, probe));
}


int CAENDigitizerBoard::GetDPP_VirtualProbe(const int& trace)
{
  int probe{0};
  CAENDigitizerException(CAEN_DGTZ_GetDPP_VirtualProbe(m_Handle,trace,&probe));
  return probe;
}

std::vector<int> CAENDigitizerBoard::GetDPP_SupportedVirtualProbes(const int& trace)
{
  int dumb{0};
  std::vector<int> probes;
  probes.reserve(MAX_SUPPORTED_PROBES);
  CAENDigitizerException(CAEN_DGTZ_GetDPP_SupportedVirtualProbes(m_Handle,trace,&probes[0],&dumb));
  probes.resize(dumb);
  return probes;
}

std::string CAENDigitizerBoard::VirtualProbeName(const int& probe)
{
  char name[256];
  CAENDigitizerException(CAEN_DGTZ_GetDPP_VirtualProbeName(probe, name));
  return name;
}

void CAENDigitizerBoard::AllocateEvent()
{
    std::cout<<"Allocate"<<std::endl;
      if(m_FamilyCode == "XX742")
      {
        CAEN_DGTZ_AllocateEvent(m_Handle,&m_Event);
      }
  
}

void CAENDigitizerBoard::SetIOLevel(const std::string& level)
{
  if(level == "NIM") CAENDigitizerException(CAEN_DGTZ_SetIOLevel(m_Handle, CAEN_DGTZ_IOLevel_NIM));
  else if(level == "TTL") CAENDigitizerException(CAEN_DGTZ_SetIOLevel(m_Handle, CAEN_DGTZ_IOLevel_TTL));
  else throw CAENDigitizerException(CAEN_DGTZ_InvalidParam);
}

std::string CAENDigitizerBoard::CAENDigitizerBoard::GetIOLevel()
{
  CAEN_DGTZ_IOLevel_t level;
  CAENDigitizerException(CAEN_DGTZ_GetIOLevel(m_Handle, &level));
  if(level == CAEN_DGTZ_IOLevel_NIM) return "NIM";
  else return "TTL";
}

void CAENDigitizerBoard::SetTriggerPolarity(const std::uint32_t& channel, const std::string&   Polarity)
{
  if(Polarity == "POSITIVE") CAENDigitizerException(CAEN_DGTZ_SetTriggerPolarity(m_Handle, channel, CAEN_DGTZ_TriggerOnRisingEdge));
  else if(Polarity == "NEGATIVE") CAENDigitizerException(CAEN_DGTZ_SetTriggerPolarity(m_Handle, channel, CAEN_DGTZ_TriggerOnFallingEdge));
  else throw CAENDigitizerException(CAEN_DGTZ_InvalidParam);
}

std::string CAENDigitizerBoard::GetTriggerPolarity(const std::uint32_t& channel)
{
  CAEN_DGTZ_TriggerPolarity_t polarity;
  CAENDigitizerException(CAEN_DGTZ_GetTriggerPolarity(m_Handle, channel, &polarity));
  if(channel == CAEN_DGTZ_TriggerOnRisingEdge) return "POSITIVE";
  else return "NEGATIVE";
}

void CAENDigitizerBoard::RearmInterrupt()
{
  CAENDigitizerException(CAEN_DGTZ_RearmInterrupt(m_Handle));
}

void CAENDigitizerBoard::SetDRS4SamplingFrequency(const std::string& freq)
{
  CAEN_DGTZ_DRS4Frequency_t frequency;
  if(freq == "5GHz") frequency = CAEN_DGTZ_DRS4_5GHz;
  else if(freq == "2.5GHz") frequency = CAEN_DGTZ_DRS4_2_5GHz;
  else if(freq == "1GHz") frequency = CAEN_DGTZ_DRS4_1GHz;
  else if(freq == "750MHz") frequency = CAEN_DGTZ_DRS4_750MHz;
  else throw CAENDigitizerException(CAEN_DGTZ_InvalidParam);
  CAENDigitizerException(CAEN_DGTZ_SetDRS4SamplingFrequency(m_Handle, frequency));
}

std::string CAENDigitizerBoard::GetDRS4SamplingFrequency()
{
  CAEN_DGTZ_DRS4Frequency_t frequency;
  CAENDigitizerException(CAEN_DGTZ_GetDRS4SamplingFrequency(m_Handle, &frequency));
  if(frequency == CAEN_DGTZ_DRS4_5GHz) return "5GHz";
  else if(frequency == CAEN_DGTZ_DRS4_2_5GHz) return "2.5GHz";
  else if(frequency == CAEN_DGTZ_DRS4_1GHz) return "1GHz";
  return "750MHz";
}

/*
 C *A*EN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetOutputSignalMode(int handle,
 CAEN_DGTZ_OutputSignalMode_t mode); CAEN_DGTZ_ErrorCode CAENDGTZ_API
 CAEN_DGTZ_GetOutputSignalMode(int handle, CAEN_DGTZ_OutputSignalMode_t *mode);
 */

void CAENDigitizerBoard::SetGroupFastTriggerThreshold(const std::uint32_t& group, const std::uint32_t& Tvalue)
{
  CAENDigitizerException(CAEN_DGTZ_SetGroupFastTriggerThreshold(m_Handle, group, Tvalue));
}

std::uint32_t CAENDigitizerBoard::GetGroupFastTriggerThreshold(const std::uint32_t& group)
{
  std::uint32_t value{0};
  CAENDigitizerException(CAEN_DGTZ_GetGroupFastTriggerThreshold(m_Handle, group, &value));
  return value;
}

void CAENDigitizerBoard::SetGroupFastTriggerDCOffset(const std::uint32_t& group, const std::uint32_t& DCvalue)
{
  CAENDigitizerException(CAEN_DGTZ_SetGroupFastTriggerDCOffset(m_Handle, group, DCvalue));
}

std::uint32_t CAENDigitizerBoard::GetGroupFastTriggerDCOffset(const std::uint32_t& group)
{
  std::uint32_t value{0};
  CAENDigitizerException(CAEN_DGTZ_GetGroupFastTriggerDCOffset(m_Handle, group, &value));
  return value;
}

void CAENDigitizerBoard::SetFastTriggerDigitizing(const bool& on)
{
  if(on == true) CAENDigitizerException(CAEN_DGTZ_SetFastTriggerDigitizing(m_Handle, CAEN_DGTZ_ENABLE));
  else CAENDigitizerException(CAEN_DGTZ_SetFastTriggerDigitizing(m_Handle, CAEN_DGTZ_DISABLE));
}

std::string CAENDigitizerBoard::GetFastTriggerDigitizing()
{
  CAEN_DGTZ_EnaDis_t enable;
  CAENDigitizerException(CAEN_DGTZ_GetFastTriggerDigitizing(m_Handle, &enable));
  if(enable == CAEN_DGTZ_ENABLE) return "ENABLED";
  else return "DISABLED";
}

void CAENDigitizerBoard::SetFastTriggerMode(const std::string& mode)
{
  CAEN_DGTZ_TriggerMode_t t;
  if(mode == "DISABLED") t = CAEN_DGTZ_TRGMODE_DISABLED;
  else if(mode == "ACQ_ONLY") t = CAEN_DGTZ_TRGMODE_ACQ_ONLY;
  else throw CAENDigitizerException(CAEN_DGTZ_InvalidParam);
  CAENDigitizerException(CAEN_DGTZ_SetFastTriggerMode(m_Handle, t));
}

std::string CAENDigitizerBoard::GetFastTriggerMode()
{
  CAEN_DGTZ_TriggerMode_t t;
  CAENDigitizerException(CAEN_DGTZ_GetFastTriggerMode(m_Handle, &t));
  if(t == CAEN_DGTZ_TRGMODE_DISABLED) return "DISABLED";
  else return "ACQ_ONLY";
}

void CAENDigitizerBoard::LoadDRS4CorrectionData()
{
  CAEN_DGTZ_DRS4Frequency_t frequency;
  if(m_DRS4Frequency == "5GHz") frequency = CAEN_DGTZ_DRS4_5GHz;
  else if(m_DRS4Frequency == "2.5GHz") frequency = CAEN_DGTZ_DRS4_2_5GHz;
  else if(m_DRS4Frequency == "1GHz") frequency = CAEN_DGTZ_DRS4_1GHz;
  else if(m_DRS4Frequency == "750MHz") frequency = CAEN_DGTZ_DRS4_750MHz;
  CAENDigitizerException(CAEN_DGTZ_LoadDRS4CorrectionData(m_Handle,frequency));
}

void CAENDigitizerBoard::GetCorrectionTables()
{
  CAEN_DGTZ_DRS4Frequency_t frequency;
  if(m_DRS4Frequency == "5GHz") frequency = CAEN_DGTZ_DRS4_5GHz;
  else if(m_DRS4Frequency == "2.5GHz") frequency = CAEN_DGTZ_DRS4_2_5GHz;
  else if(m_DRS4Frequency == "1GHz") frequency = CAEN_DGTZ_DRS4_1GHz;
  else if(m_DRS4Frequency == "750MHz") frequency = CAEN_DGTZ_DRS4_750MHz;
  CAENDigitizerException(CAEN_DGTZ_GetCorrectionTables(m_Handle,frequency,&std::any_cast<std::array<CAEN_DGTZ_DRS4Correction_t,MAX_X742_GROUP_SIZE>>(m_DRS4Correction)[0]));
}

void CAENDigitizerBoard::EnableDRS4Correction()
{
  CAENDigitizerException(CAEN_DGTZ_EnableDRS4Correction(m_Handle));
}

void CAENDigitizerBoard::DisableDRS4Correction()
{
  CAENDigitizerException(CAEN_DGTZ_DisableDRS4Correction(m_Handle));
}
 /*CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_DecodeZLEWaveforms(int handle, void *event, void *waveforms); 
 CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_FreeZLEWaveforms(int handle, void *waveforms);
 CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_MallocZLEWaveforms(int handle, void **waveforms,uint32_t *allocatedSize); 
 CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_FreeZLEEvents(int handle, void **events); 
 CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_MallocZLEEvents(int handle, void **events, uint32_t*allocatedSize); 
 CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetZLEEvents(int handle, char *buffer, uint32_t buffsize, void **events, uint32_t* numEventsArray); 
 CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetZLEParameters(int handle, uint32_t channelMask, void* params);
 CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetSAMCorrectionLevel(int handle,CAEN_DGTZ_SAM_CORRECTION_LEVEL_t *level); 
 CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetSAMCorrectionLevel(int handle, CAEN_DGTZ_SAM_CORRECTION_LEVEL_t level); 
 CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_EnableSAMPulseGen(int handle, int channel, unsigned short  pulsePattern, CAEN_DGTZ_SAMPulseSourceType_t pulseSource); 
 CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_DisableSAMPulseGen(int handle, int channel); 
 CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetSAMPostTriggerSize(int handle, int SamIndex, uint8_t value); 
 CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetSAMPostTriggerSize(int handle, int SamIndex, uint32_t *value); 
 CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetSAMSamplingFrequency(int handle, CAEN_DGTZ_SAMFrequency_t frequency); 
 CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetSAMSamplingFrequency(int handle, CAEN_DGTZ_SAMFrequency_t *frequency); 
 CAEN_DGTZ_ErrorCode CAENDGTZ_API _CAEN_DGTZ_Read_EEPROM(int handle, int EEPROMIndex, unsigned short add, int nbOfBytes, unsigned char* buf); 
 CAEN_DGTZ_ErrorCode CAENDGTZ_API _CAEN_DGTZ_Write_EEPROM(int handle, int EEPROMIndex, unsigned short add, int nbOfBytes, void* buf);
 CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_LoadSAMCorrectionData(int handle);
 CAEN_DGTZ_ErrorCode CAENDGTZ_API _CAEN_DGTZ_TriggerThreshold(int handle, CAEN_DGTZ_EnaDis_t endis); 
 CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SendSAMPulse(int handle); 
 CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetSAMAcquisitionMode(int handle, CAEN_DGTZ_AcquisitionMode_t mode);
 CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetSAMAcquisitionMode(int handle, CAEN_DGTZ_AcquisitionMode_t *mode); 
 CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetTriggerLogic(int handle, CAEN_DGTZ_TrigerLogic_t logic,  uint32_t majorityLevel); 
 CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetTriggerLogic(int handle, CAEN_DGTZ_TrigerLogic_t *logic, uint32_t *majorityLevel);
 CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetChannelPairTriggerLogic(int handle,  uint32_t channelA, uint32_t channelB, CAEN_DGTZ_TrigerLogic_t *logic, uint16_t *coincidenceWindow); 
 CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetChannelPairTriggerLogic(int handle,  uint32_t channelA, uint32_t channelB, CAEN_DGTZ_TrigerLogic_t logic, uint16_t coincidenceWindow);
 */

void CAENDigitizerBoard::SetDecimationFactor(const std::uint16_t& factor)
{
  CAENDigitizerException(CAEN_DGTZ_SetDecimationFactor(m_Handle, factor));
}

std::uint16_t CAENDigitizerBoard::GetDecimationFactor()
{
  std::uint16_t factor{0};
  CAENDigitizerException(CAEN_DGTZ_GetDecimationFactor(m_Handle, &factor));
  return factor;
}

/*
 CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetSAMTriggerCountVetoParam(int handle, int channel, CAEN_DGTZ_EnaDis_t enable, uint32_t vetoWindow);
 CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetSAMTriggerCountVetoParam(int handle, int channel, CAEN_DGTZ_EnaDis_t *enable, uint32_t *vetoWindow);
 CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetSAMCorrectionData(int handle,void *STable);*/

void CAENDigitizerBoard::Calibrate()
{
  CAENDigitizerException(CAEN_DGTZ_Calibrate(m_Handle));
}

std::uint32_t CAENDigitizerBoard::ReadTemperature(const std::int32_t& ch)
{
  std::uint32_t temp{0};
  CAENDigitizerException(CAEN_DGTZ_ReadTemperature(m_Handle, ch, &temp));
  return temp;
}

std::string CAENDigitizerBoard::GetDPPFirmwareType()
{
  CAEN_DGTZ_DPPFirmware_t firmware;
  CAENDigitizerException(CAEN_DGTZ_GetDPPFirmwareType(m_Handle, &firmware));
  switch(firmware)
  {
    case CAEN_DGTZ_DPPFirmware_PHA: return "PHA";
    case CAEN_DGTZ_DPPFirmware_PSD: return "PSD";
    case CAEN_DGTZ_DPPFirmware_CI: return "CI";
    case CAEN_DGTZ_DPPFirmware_ZLE: return "ZLE";
    case CAEN_DGTZ_DPPFirmware_QDC: return "QDC";
    case CAEN_DGTZ_DPPFirmware_DAW: return "DAW";
    case CAEN_DGTZ_NotDPPFirmware: return "NODPP";
    default: return "NODPP";
  }
}

void CAENDigitizerBoard::LoadDACCalibration()
{
  if(m_FamilyCode=="XX742") return;
  Flash flash;
  flash.setHandle(m_Handle);
  flash.init();
  std::vector<uint8_t> buf=flash.read_virtual_page();
  if(buf[0] != 0xD)
  {
    sendWarning("No DAC Calibration data found in board flash. Use option 'D' to calibrate DAC.");
  }
  else
  {
    for(std::size_t ch = 0; ch < m_Nch; ++ch)
    {
      m_Cal[ch]    = static_cast<float>(buf[2 * ch]);
      m_Offset[ch] = static_cast<float>(buf[1 + 2 * ch]);
    }
  }
  sendInfo("DAC calibration correctly loaded from board flash.");
}

void CAENDigitizerBoard::PerformCalibration()
{
  if(!m_StartupCalibration)return;
  if(BoardSupportsCalibration())
  {
    if(getState()!=States::STARTED)
    {
      try
      {
        Calibrate();
      }
      catch(const CAENDigitizerException& error)
      {
        sendError("ADC Calibration failed. CAENDigitizer ERR ");
        sendError(error.what());
      }
      sendInfo("ADC Calibration successfully executed.");
    }
    else sendWarning("Can't run ADC calibration while acquisition is running.");
  }
  else sendInfo("ADC Calibration not needed for this board family.");
}

void CAENDigitizerBoard::MaskChannels()
{
  if((m_FamilyCode != "XX740") && (m_FamilyCode != "XX742")) { m_EnableMask &= (1 << m_Nch) - 1; }
  else
  {
    m_EnableMask &= (1 << (m_Nch / 8)) - 1;
  }
  if((m_FamilyCode == "XX751") && m_DesMode=="ENABLED") { m_EnableMask &= 0xAA; }
  if((m_FamilyCode == "XX731") && m_DesMode=="ENABLED") { m_EnableMask &= 0x55; }
}

void CAENDigitizerBoard::Read()
{
  ReadData();
  if(m_BufferSize == 0)
  {
    uint32_t lstatus = ReadRegister(CAEN_DGTZ_ACQ_STATUS_ADD);
    if(lstatus & (0x1 << 19)) Exception(StatusCode::FAILURE,"Over Temperature");
  }
}



///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//////// Board functions
void  CAENDigitizerBoard::DoInitialize()
{
}

void  CAENDigitizerBoard::DoConfigure()
{
  GetInfo();
  sendInfo("Connected to CAEN Digitizer Model {}",m_ModelName);
  sendInfo("ROC FPGA Release is  {}",m_ROC_FirmwareRel);
  sendInfo("AMC FPGA Release is {}",m_AMC_FirmwareRel);
  // Check firmware rivision (DPP firmwares cannot be used with WaveDump */
  if(m_DPPFirmware) Exception(StatusCode::FAILURE,"This digitizer has a DPP firmware");
  GetMoreBoardInfo();
  LoadDACCalibration();
  PerformCalibration();
  MaskChannels();
  ProgramDigitizer();
  // Read again the board infos, just in case some of them were changed by
  // the programming (like, for example, the TSample and the number of
  // channels if DES mode is changed)
  GetInfo();
  GetMoreBoardInfo();
  // Reload Correction Tables if changed
  if(m_FamilyCode == "XX742")
  {
    if(m_UseCorrections==true)
    {  
      m_DRS4Correction=std::make_any<std::array<CAEN_DGTZ_DRS4Correction_t,MAX_X742_GROUP_SIZE>>();
      GetCorrectionTables();
      // Use Manual Corrections
      // Disable Automatic Corrections
      DisableDRS4Correction();
      // Load the Correction Tables from the Digitizer flash
      
      
      /*if(dat.WDcfg.UseManualTables != -1)
      {  // The user wants to use some custom tables
        uint32_t gr;
        uint32_t GroupMask = dat.WDcfg.UseManualTables;
        
        for(gr = 0; gr < dat.WDcfg.MaxGroupNumber; gr++)
        {
          if(((GroupMask >> gr) & 0x1) == 0) continue;
          LoadCorrectionTable(dat.WDcfg.TablesFilenames[gr], &(X742Tables[gr]));
        }
      }
      // Save to file the Tables read from flash
      GroupMask = (~GroupMask) & ((0x1 << dat.WDcfg.MaxGroupNumber) - 1);
      SaveCorrectionTables("X742Table", GroupMask, X742Tables);*/
    }
    else
    {  
      // Use Automatic Corrections
      LoadDRS4CorrectionData();
      EnableDRS4Correction();
    }
  }
}

void  CAENDigitizerBoard::DoAtFirstStart()
{
  MallocReadoutBuffer();
  AllocateEvent();
}

void  CAENDigitizerBoard::DoEvent()
{
  
}

void  CAENDigitizerBoard::DoLoopOnStart()
{
  SWStartAcquisition();
  // Send a software trigger //
  if(m_ContinuousTrigger) SendSWtrigger();
  /// Wait for interrupt (if enabled) //
  if(m_InterruptNumEvents>0)
  {
    if(Interrupt()==true) DoEvent();
  }
  // Read data from the board //
  Read();
  for(std::size_t event = 0; event < GetNumEvents(); event++) 
  {
    EventInfo eventInfos = GetEventInfo(event);
    DecodeEvent(eventInfos.getEvent());
    //CAEN_DGTZ_X742_EVENT_t* p=reinterpret_cast<CAEN_DGTZ_X742_EVENT_t*>(m_Event);
    Json::Value j;
    j["EventInfos"]["EventSize"]=eventInfos.getEventSize();
    j["EventInfos"]["BoardID"]=eventInfos.getBoardId();
    j["EventInfos"]["Pattern"]=eventInfos.getPattern();
    j["EventInfos"]["ChannelMask"]=eventInfos.getChannelMask();
    m_EventNumber++;
    j["EventInfos"]["EventCounter"]=m_EventNumber;
    j["EventInfos"]["TriggerTimeTag"]=eventInfos.getTriggerTimeTag();
    j["Event"]=parse(reinterpret_cast<CAEN_DGTZ_X742_EVENT_t*>(m_Event));
    
    
    //std::cout<<j<<std::endl;
    
    //std::cout<<parse(reinterpret_cast<CAEN_DGTZ_X742_EVENT_t*>(m_Event))<<std::endl;
    
    /**reinterpret_cast<CAEN_DGTZ_X742_EVENT_t*>(m_Event);*/
  //  std::string toto=j.dump();
 // std::cout<<j.dump().c_str()<<std::endl;
    sendData(j);
    
    
  //  Parse();
  }
}

void CAENDigitizerBoard::Parse(){}

void  CAENDigitizerBoard::DoStart()
{
  LoopOnStart();
}

void  CAENDigitizerBoard::DoStop()
{
  m_EventNumber=0;
  SWStopAcquisition();
  FreeReadoutBuffer();
  FreeEvent();
}

void  CAENDigitizerBoard::DoLoopOnPause()
{
}


void  CAENDigitizerBoard::DoPause()
{
  
  SWStopAcquisition();
}

void CAENDigitizerBoard::setModelName(const std::string& name)
{
  m_ModelName = name;
}

void CAENDigitizerBoard::setModel(const std::uint32_t& model)
{
  switch(model)
  {
    case CAEN_DGTZ_V1724: m_Model = "V1724"; break; /*!< \brief The board is V1724  */
    case CAEN_DGTZ_V1721: m_Model = "V1721"; break; /*!< \brief The board is V1721  */
    case CAEN_DGTZ_V1731: m_Model = "V1731"; break; /*!< \brief The board is V1731  */
    case CAEN_DGTZ_V1720: m_Model = "V1720"; break; /*!< \brief The board is V1720  */
    case CAEN_DGTZ_V1740: m_Model = "V1740"; break; /*!< \brief The board is V1740  */
    case CAEN_DGTZ_V1751: m_Model = "V1751"; break; /*!< \brief The board is V1751  */
    case CAEN_DGTZ_DT5724: m_Model = "DT5724"; break; /*!< \brief The board is DT5724 */
    case CAEN_DGTZ_DT5721: m_Model = "DT5721"; break; /*!< \brief The board is DT5721 */
    case CAEN_DGTZ_DT5731: m_Model = "DT5731"; break; /*!< \brief The board is DT5731 */
    case CAEN_DGTZ_DT5720: m_Model = "DT5720"; break; /*!< \brief The board is DT5720 */
    case CAEN_DGTZ_DT5740: m_Model = "DT5740"; break; /*!< \brief The board is DT5740 */
    case CAEN_DGTZ_DT5751: m_Model = "DT5751"; break; /*!< \brief The board is DT5751 */
    case CAEN_DGTZ_N6724: m_Model = "N6724"; break; /*!< \brief The board is N6724  */
    case CAEN_DGTZ_N6721: m_Model = "N6721"; break; /*!< \brief The board is N6721  */
    case CAEN_DGTZ_N6731: m_Model = "N6731"; break; /*!< \brief The board is N6731  */
    case CAEN_DGTZ_N6720: m_Model = "N6720"; break; /*!< \brief The board is N6720  */
    case CAEN_DGTZ_N6740: m_Model = "N6740"; break; /*!< \brief The board is N6740  */
    case CAEN_DGTZ_N6751: m_Model = "N6751"; break; /*!< \brief The board is N6751  */
    case CAEN_DGTZ_DT5742: m_Model = "DT5742"; break; /*!< \brief The board is DT5742 */
    case CAEN_DGTZ_N6742: m_Model = "N6742"; break; /*!< \brief The board is N6742  */
    case CAEN_DGTZ_V1742: m_Model = "V1742"; break; /*!< \brief The board is V1742  */
    case CAEN_DGTZ_DT5780: m_Model = "DT5780"; break; /*!< \brief The board is DT5780 */
    case CAEN_DGTZ_N6780: m_Model = "N6780"; break; /*!< \brief The board is N6780  */
    case CAEN_DGTZ_V1780: m_Model = "V1780"; break; /*!< \brief The board is V1780  */
    case CAEN_DGTZ_DT5761: m_Model = "DT5761"; break; /*!< \brief The board is DT5761 */
    case CAEN_DGTZ_N6761: m_Model = "N6761"; break; /*!< \brief The board is N6761  */
    case CAEN_DGTZ_V1761: m_Model = "V1761"; break; /*!< \brief The board is V1761  */
    case CAEN_DGTZ_DT5743: m_Model = "DT5743"; break; /*!< \brief The board is DT5743 */
    case CAEN_DGTZ_N6743: m_Model = "N6743"; break; /*!< \brief The board is N6743  */
    case CAEN_DGTZ_V1743: m_Model = "V1743"; break; /*!< \brief The board is V1743  */
    case CAEN_DGTZ_DT5730: m_Model = "DT5730"; break; /*!< \brief The board is DT5730 */
    case CAEN_DGTZ_N6730: m_Model = "N6730"; break; /*!< \brief The board is N6730  */
    case CAEN_DGTZ_V1730: m_Model = "V1730"; break; /*!< \brief The board is V1730  */
    case CAEN_DGTZ_DT5790: m_Model = "DT5790"; break; /*!< \brief The board is DT5790 */
    case CAEN_DGTZ_N6790: m_Model = "N6790"; break; /*!< \brief The board is N6790  */
    case CAEN_DGTZ_V1790: m_Model = "V1790"; break; /*!< \brief The board is V1790  */
    case CAEN_DGTZ_DT5781: m_Model = "DT5791"; break; /*!< \brief The board is DT5781 */
    case CAEN_DGTZ_N6781: m_Model = "N6781"; break; /*!< \brief The board is N6781  */
    case CAEN_DGTZ_V1781: m_Model = "V1781"; break; /*!< \brief The board is V1781  */
    case CAEN_DGTZ_DT5725: m_Model = "DT5725"; break; /*!< \brief The board is DT5725 */
    case CAEN_DGTZ_N6725: m_Model = "N6725"; break; /*!< \brief The board is N6725  */
    case CAEN_DGTZ_V1725: m_Model = "V1725"; break; /*!< \brief The board is V1725  */
    case CAEN_DGTZ_V1782: m_Model = "V1782"; break; /*!< \brief The board is V1782  */
    default: m_Model = "";
  }
}

void CAENDigitizerBoard::setNbrChannels(const std::uint32_t& nbr)
{
  m_NbrChannels = nbr;
}

void CAENDigitizerBoard::setFormFactor(const std::uint32_t& form)
{
  switch(form)
  {
    case CAEN_DGTZ_VME64_FORM_FACTOR: m_FormFactor = "VME64"; break;
    case CAEN_DGTZ_VME64X_FORM_FACTOR: m_FormFactor = "VME64X"; break;
    case CAEN_DGTZ_DESKTOP_FORM_FACTOR: m_FormFactor = "DESKTOP"; break;
    case CAEN_DGTZ_NIM_FORM_FACTOR: m_FormFactor = "NIM"; break;
    default: m_FormFactor = "";
  }
}

void CAENDigitizerBoard::setFamilyCode(const std::uint32_t& fam)
{
  switch(fam)
  {
    case CAEN_DGTZ_XX724_FAMILY_CODE: m_FamilyCode = "XX724"; break;
    case CAEN_DGTZ_XX721_FAMILY_CODE: m_FamilyCode = "XX721"; break;
    case CAEN_DGTZ_XX731_FAMILY_CODE: m_FamilyCode = "XX731"; break;
    case CAEN_DGTZ_XX720_FAMILY_CODE: m_FamilyCode = "XX720"; break;
    case CAEN_DGTZ_XX740_FAMILY_CODE: m_FamilyCode = "XX740"; break;
    case CAEN_DGTZ_XX751_FAMILY_CODE: m_FamilyCode = "XX751"; break;
    case CAEN_DGTZ_XX742_FAMILY_CODE: m_FamilyCode = "XX742"; break;
    case CAEN_DGTZ_XX780_FAMILY_CODE: m_FamilyCode = "XX780"; break;
    case CAEN_DGTZ_XX761_FAMILY_CODE: m_FamilyCode = "XX761"; break;
    case CAEN_DGTZ_XX743_FAMILY_CODE: m_FamilyCode = "XX743"; break;
    // NOTE: 10 is skipped because maps family models DT55xx
    case CAEN_DGTZ_XX730_FAMILY_CODE: m_FamilyCode = "XX730"; break;
    case CAEN_DGTZ_XX790_FAMILY_CODE: m_FamilyCode = "XX790"; break;
    case CAEN_DGTZ_XX781_FAMILY_CODE: m_FamilyCode = "XX781"; break;
    case CAEN_DGTZ_XX725_FAMILY_CODE: m_FamilyCode = "XX725"; break;
    case CAEN_DGTZ_XX782_FAMILY_CODE: m_FamilyCode = "XX782"; break;
    default: m_FamilyCode = "";
  }
}

void CAENDigitizerBoard::setROCFirmwareRel(const std::string& firm)
{
  m_ROC_FirmwareRel = firm;
}

void CAENDigitizerBoard::setAMCFirmwareRel(const std::string& firm)
{
  m_AMC_FirmwareRel = firm;
}

void CAENDigitizerBoard::setSerialNumber(const std::uint32_t& serial)
{
  m_SerialNumber = serial;
}

void CAENDigitizerBoard::setMezzanineSerialNumber(char serials[4][8])
{
  m_MezzanineSerialNumber[0] = serials[0];
  m_MezzanineSerialNumber[1] = serials[1];
  m_MezzanineSerialNumber[2] = serials[2];
  m_MezzanineSerialNumber[3] = serials[3];
}

void CAENDigitizerBoard::setPCBRevision(const std::uint32_t& pcb)
{
  m_PCB_Revision = pcb;
}

void CAENDigitizerBoard::setADCNBits(const std::uint32_t& nbits)
{
  m_ADC_NBits = nbits;
}

void CAENDigitizerBoard::setSAMCorrectionDataLoaded(const std::uint32_t& sam)
{
  m_SAMCorrectionDataLoaded = sam;
}

void CAENDigitizerBoard::setLicense(const std::string& lic)
{
  m_License = lic;
}

void CAENDigitizerBoard::setCommHandle(const std::uint32_t& han)
{
  m_CommHandle = han;
}

void CAENDigitizerBoard::setVMEHandle(const std::uint32_t& han)
{
  m_VMEHandle = han;
}

void CAENDigitizerBoard::setHasDPPFirware()
{
  std::size_t found = m_AMC_FirmwareRel.find(".");
  if(std::stoi(m_AMC_FirmwareRel.substr(0,found))>128) m_DPPFirmware=true;
}

void CAENDigitizerBoard::initilizeParameters()
{
  m_Cal.fill(1);
  m_Offset.fill(0);
  m_PulsePolarity.fill("POSITIVE");
  m_Version_used.fill(0);
  m_DCoffset.fill(0);
  m_Threshold.fill(0);
  m_ChannelTriggerMode.fill("DISABLED");
  m_GroupTrgEnableMask.fill(0);
  for(std::size_t i = 0; i < m_DCoffsetGrpCh.size(); i++) m_DCoffsetGrpCh[i].fill(-1);
  for(std::size_t i = 0; i < m_TablesFilenames.size(); i++) m_TablesFilenames[i] = "Tables_gr" + std::to_string(i);
  m_FTThreshold.fill(0);
  m_FTDCoffset.fill(0);
}

void CAENDigitizerBoard::verifyParameters()
{
  bool change=false;
  // Save previous values (for compare)
  std::string                PrevDesMode         = m_DesMode;
  bool                       PrevUseCorrections  = m_UseCorrections;
  bool                       PrevUseManualTables = m_UseManualTables;
  std::array<std::string, 4> PrevTablesFilenames = m_TablesFilenames;
  std::string PrevDRS4Freq=m_DRS4Frequency;
  //WRITE_REGISTER
  try
  {
    const auto writeRegisters = toml::find(m_Conf,"WRITE_REGISTER");
    try
    {
      m_WriteRgisters = toml::get<std::vector<std::array<uint32_t,3>>>(writeRegisters);
    }
    catch(...)
    {
      throw Exception(StatusCode::INVALID_PARAMETER,"WRITE_REGISTER should be of the form [[address,data,mask],...]");
    }
  }
  catch(...){}
  
  //RECORD_LENGTH
  m_RecordLength = toml::find_or(m_Conf,"RECORD_LENGTH",1024);
  if(m_RecordLength>1024)
  {
    throw Exception(StatusCode::INVALID_PARAMETER,"RECORD_LENGTH should not be > 1024");
  }
  else if(m_FamilyCode=="XX742")
  {
    if(m_RecordLength!=1024&&m_RecordLength!=520&&m_RecordLength!=256&&m_RecordLength!=136)
    {
      throw Exception(StatusCode::INVALID_PARAMETER,"RECORD_LENGTH can be only 1024, 520, 256 or 136 for XX742 familly");
    }
  }
  
  //DRS4_FREQUENCY
  m_DRS4Frequency =toml::find_or<std::string>(m_Conf, "DRS4_FREQUENCY","5GHz");
  if(m_DRS4Frequency != "5GHz" && m_DRS4Frequency != "2.5GHz" && m_DRS4Frequency != "1GHz" &&m_DRS4Frequency != "750MHz")
  {
    throw Exception(StatusCode::INVALID_PARAMETER,"DRS4_FREQUENCY can be 5GHz 2.5GHz 1GHz or 750MHz");
  }
  if(PrevDRS4Freq != m_DRS4Frequency) change=true;
  
  //FIXME CORRECTION LEVEL
  
  //TEST_PATTERN
  m_Test = toml::find_or(m_Conf, "TEST_PATTERN", false);
  
  //DECIMATION FACTOR
  m_DecimationFactor = toml::find_or<std::uint16_t>(m_Conf, "DECIMATION_FACTOR", 1);
  if(m_DecimationFactor != 1 && m_DecimationFactor != 2 && m_DecimationFactor != 4 && m_DecimationFactor != 8 && m_DecimationFactor != 16 && m_DecimationFactor != 32 && m_DecimationFactor != 64 && m_DecimationFactor != 128)
  {
    throw Exception(StatusCode::INVALID_PARAMETER,"DECIMATION_FACTOR can be 1 2 4 8 16 32 64 128 only");
  }
  
  //EXTERNAL_TRIGGER
  m_ExtTriggerMode = toml::find_or<std::string>(m_Conf, "EXTERNAL_TRIGGER", "ACQ_ONLY");
  if(m_ExtTriggerMode != "DISABLED" && m_ExtTriggerMode != "ACQ_ONLY" && m_ExtTriggerMode != "ACQ_AND_EXTOUT")
  {
    throw Exception(StatusCode::INVALID_PARAMETER,"EXTERNAL_TRIGGER can be DISABLED ACQ_ONLY ACQ_AND_EXTOUT");
  }
  std::cout<<"Set "<<m_ExtTriggerMode<<std::endl;
  //MAX_NUM_EVENTS_BLT
  m_NumberEventBLT= toml::find_or(m_Conf,"MAX_NUM_EVENTS_BLT",1023);
  if(m_NumberEventBLT>1023)
  {
    throw Exception(StatusCode::INVALID_PARAMETER,"MAX_NUM_EVENTS_BLT should not be > 1023");
  }
  
  //POST_TRIGGER
  m_PostTrigger=toml::find_or<int>(m_Conf, "POST_TRIGGER",50);
  if(m_PostTrigger<0||m_PostTrigger>100)
  {
    spdlog::warn("POST_TRIGGER must be between 0% to 100%");
  }
  
  //FIXME ENABLE_DES_MODE
  
  //FIXME SHOULDNT BE HERE OUTPUT_FILE_FORMAT
  m_FileFormat=toml::find_or<std::string>(m_Conf, "OUTPUT_FILE_FORMAT", "ROOT");
  if(m_FileFormat != "ASCII" && m_FileFormat != "ROOT" && m_FileFormat != "BINARY")
  {
    throw Exception(StatusCode::INVALID_PARAMETER,"OUTPUT_FILE_FORMAT can be ROOT, ASCII or BINARY");
  }

  //FIXME SHOULDNT BE HERE OUTPUT_FILE_HEADER
  m_Header = toml::find_or(m_Conf, "OUTPUT_FILE_HEADER", true);
  
  //USE_interrupt
  m_InterruptNumEvents=toml::find_or<int>(m_Conf, "USE_INTERRUPT",0);
  
  //FAST_TRIGGER
  m_FastTriggerMode =toml::find_or<std::string>(m_Conf, "FAST_TRIGGER","DISABLED");
  if(m_FastTriggerMode != "DISABLED" && m_FastTriggerMode != "ACQ_ONLY")
  {
    throw Exception(StatusCode::INVALID_PARAMETER,"FAST_TRIGGER can be DISABLED or ACQ_ONLY");
  }
  
  //ENABLED_FAST_TRIGGER_DIGITIZING
  m_FastTriggerEnabled=toml::find_or<bool>(m_Conf, "ENABLED_FAST_TRIGGER_DIGITIZING",true);
  
  //PULSE_POLARITY
  std::string pulsePolarity =toml::find_or<std::string>(m_Conf, "PULSE_POLARITY","NEGATIVE");
  if(pulsePolarity!= "NEGATIVE" && pulsePolarity != "POSITIVE")
  {
    throw Exception(StatusCode::INVALID_PARAMETER,"PULSE_POLARITY can be POSITIVE or NEGATIVE");
  }
  for(size_t i=0;i!=m_MAX_SET;++i) m_PulsePolarity[i]=pulsePolarity;
  
  //DC_OFFSET
  //FOR TRIGGER
  for(std::size_t trigger=0;trigger!=m_MAX_SET;++trigger)
  {
    int dc_offset =toml::find_or<int>(m_Conf, "DC_OFFSET_TR"+std::to_string(trigger),0);
    m_FTDCoffset[trigger* 2]     = (uint32_t)dc_offset;
    m_FTDCoffset[trigger*2+1]     = (uint32_t)dc_offset;
    std::cout<<m_FTDCoffset[trigger* 2]<<std::endl;
  }
  //COMMON
  int val=toml::find_or<int>(m_Conf, "DC_OFFSET_COMMON",0);
  if(val!=0)
  {
    int val2=(int)((val + 50) * 65535 / 100);
    for(std::size_t channel=0;channel!=m_MAX_SET;++channel)
    {
      m_DCoffset[channel]=val2;
      m_Version_used[channel]=0;
      m_DCfile[channel]=val;
    }
  }
  //FOR CHANNEL
  for(std::size_t channel=0;channel!=m_MAX_SET;++channel)
  {
    int dc =toml::find_or<int>(m_Conf, "DC_OFFSET_CHANNEL"+std::to_string(channel),0);
    int val2 = (int)((dc + 50) * 65535 / 100);
    m_DCoffset[channel]=val2;
    m_Version_used[channel]=0;
    m_DCfile[channel]=dc;
  }
  
  //BASELINE_LEVEL
  //FOR TRIGGER
  for(std::size_t trigger=0;trigger!=m_MAX_SET/2;++trigger)
  {
    int dc_offset =toml::find_or<int>(m_Conf, "BASELINE_LEVEL_TR"+std::to_string(trigger),0);
    if(dc_offset!=0)
    {
      m_FTDCoffset[trigger* 2]     = (uint32_t)dc_offset;
      m_FTDCoffset[trigger*2+1]     = (uint32_t)dc_offset;
    }
  }
  //COMMON
  val=toml::find_or<int>(m_Conf, "BASELINE_LEVEL_COMMON",0);
  if(val!=0)
  {
    for(std::size_t channel=0;channel!=m_MAX_SET;++channel)
    {
      m_Version_used[channel]=1;
      m_DCfile[channel]=val;
      if(m_PulsePolarity[channel] == "POSITIVE") m_DCoffset[channel] = (uint32_t)((float)(std::fabs(val - 100)) * (655.35));
      else if(m_PulsePolarity[channel] == "NEGATIVE") m_DCoffset[channel] = (uint32_t)((float)(val) * (655.35));
    }
  }
  //FOR CHANNEL
  for(std::size_t channel=0;channel!=m_MAX_SET;++channel)
  {
    int val =toml::find_or<int>(m_Conf, "BASELINE_LEVEL_CHANNEL"+std::to_string(channel),0);
    if(val!=0)
    {
      if(m_PulsePolarity[channel] == "POSITIVE") m_DCoffset[channel] = (uint32_t)((float)(std::fabs(val - 100)) * (655.35));
      else if(m_PulsePolarity[channel] == "NEGATIVE") m_DCoffset[channel] = (uint32_t)((float)(val) * (655.35));
      m_Version_used[channel]=1;
      m_DCfile[channel]=val;
    }
  }
  
  
  //GRP_CH_DC_OFFSET
  for(std::size_t group=0;group!=m_MAX_SET;++group)
  {
    std::string val =toml::find_or<std::string>(m_Conf, "GRP_CH_DC_OFFSET","");
    if(val!="")
    {
      std::vector<float> dc;
      tokenize(val,dc,",");
      if(dc.size()!=8) throw Exception(StatusCode::WRONG_NUMBER_PARAMETERS,"GRP_CH_DC_OFFSET should be one the form \"0.,0.,0.,0.,0.,0.,0.,0.\"");
      for(std::size_t i=0;i!=dc.size();++i)
      {
        m_DC8file[i]=dc[i];
        int val2 = (int)((dc[i] + 50) * 65535 / 100);  /// DC offset (percent of the dynamic range, -50 to 50)
        m_DCoffsetGrpCh[group][i];
      }
    }
  }
  
  
  //TRIGGER_THRESHOLD
  //FOR TRIGGER
  for(std::size_t trigger=0;trigger!=m_MAX_SET/2;++trigger)
  {
    int dc_offset =toml::find_or<int>(m_Conf, "TRIGGER_THRESHOLD_TR"+std::to_string(trigger),0);
    m_FTThreshold[trigger* 2]     = (uint32_t)dc_offset;
    m_FTThreshold[trigger*2+1]     = (uint32_t)dc_offset;
    std::cout<<m_FTThreshold[trigger* 2]<<std::endl;
  }
  //COMMON
  val=toml::find_or<int>(m_Conf, "TRIGGER_THRESHOLD_COMMON",0);
  if(val!=0)
  {
    for(std::size_t channel=0;channel!=m_MAX_SET;++channel)
    {
      m_Threshold[channel]=val;
      m_Thrfile[channel]=val;
    }
  }
  //FOR CHANNEL
  for(std::size_t channel=0;channel!=m_MAX_SET;++channel)
  {
    int val =toml::find_or<int>(m_Conf, "TRIGGER_THRESHOLD_CHANNEL"+std::to_string(channel),0);
    m_Threshold[channel]=val;
    m_Thrfile[channel]=val;
  }
  

  
  
  m_FPIOtype    = toml::find_or<std::string>(m_Conf, "FPIO_LEVEL", "NIM");
  if(m_FPIOtype!="NIM"&&m_FPIOtype!="TTL")
  {
    throw Exception(StatusCode::INVALID_PARAMETER,"FPIO_LEVEL can be NIM or TTL");
  }



  


  
  m_StartupCalibration=toml::find_or<bool>(m_Conf, "SKIP_STARTUP_CALIBRATION",false);
  
}







void CAENDigitizerBoard::Set_calibrated_DCO(const int& ch)
{
  /*old DC_OFFSET config, skip calibration*/
  if(m_Version_used[ch] == 0) return;
  if(m_PulsePolarity[ch] == "POSITIVE")
  {
    m_DCoffset[ch] = (uint32_t)std::fabs(((m_DCfile[ch] - m_Offset[ch]) / m_Cal[ch]) - 100.) *(655.35);
  }
  else if(m_PulsePolarity[ch] == "NEGATIVE")
  {
    m_DCoffset[ch] =(uint32_t)(std::fabs(((std::fabs(m_DCfile[ch] - 100.) - m_Offset[ch]) / m_Cal[ch]) -100.)) *(655.35);
  }
  if(m_DCoffset[ch] > 65535) m_DCoffset[ch] = 65535;
  if(m_DCoffset[ch] < 0) m_DCoffset[ch] = 0;
  try
  {
    if(m_FamilyCode == "XX740")
    {
      SetGroupDCOffset((uint32_t)ch, m_DCoffset[ch]);
    }
    else
    {
      SetChannelDCOffset((uint32_t)ch, m_DCoffset[ch]);
    }
  }
  catch(const Exception& error)
  {
    throw Exception(StatusCode::INVALID_PARAMETER,"Error setting group "+std::to_string(ch)+" offset");
  }
}

void CAENDigitizerBoard::WriteRegisterBitmask(const std::uint32_t& address,const std::uint32_t& data,const std::uint32_t& mask)
{
  std::uint32_t d32 = ReadRegister(address);
  std::uint32_t dat = data;
  dat &= mask;
  d32 &= ~mask;
  d32 |= dat;
  WriteRegister(address, d32);
}

void CAENDigitizerBoard::ProgramDigitizer()
{
  /* reset the digitizer */
  try
  {
    Reset();
  }
  catch(const Exception& error)
  {
    sendError("Unable to reset digitizer !\nPlease reset digitizer manually then restart the program !");
    throw;
  }
  try
  {
    // Set the waveform test bit for debugging
    if(m_Test == true)
    {
      WriteRegister(CAEN_DGTZ_BROAD_CH_CONFIGBIT_SET_ADD, 1 << 3);
    }
    // custom setting for X742 boards
    if(m_FamilyCode == "XX742")
    {
      SetFastTriggerDigitizing(m_FastTriggerEnabled);
      SetFastTriggerMode(m_FastTriggerMode);
    }
    if(m_FamilyCode == "XX751" || m_FamilyCode == "XX731"){ SetDESMode(m_DesMode); }
    SetRecordLength(m_RecordLength);
    m_RecordLength = GetRecordLength();
    if(m_FamilyCode == "XX740" || m_FamilyCode == "XX724"){ SetDecimationFactor(m_DecimationFactor); }
    SetPostTriggerSize(m_PostTrigger);
    if(m_FamilyCode != "XX742") { m_PostTrigger = GetPostTriggerSize(); }
    SetIOLevel(m_FPIOtype);
    if(m_InterruptNumEvents > 0)
    {
      // Interrupt handling
      try
      {
        InterruptConfig interruptConfig;
        interruptConfig.setLevel(m_VME_INTERRUPT_LEVEL);
        interruptConfig.setStatusID(m_VME_INTERRUPT_STATUS_ID);
        interruptConfig.setEventNumber(m_InterruptNumEvents);
        interruptConfig.setMode("ROAK");
        interruptConfig.setState("ENABLED");
        SetInterruptConfig(interruptConfig);
      }
      catch(const Exception& error)
      {
        sendError("Error configuring interrupts. Interrupts disabled");
        m_InterruptNumEvents = 0;
      }
    }
    SetMaxNumEventsBLT(m_NumberEventBLT);
    std::cout<<"MAX"<<GetMaxNumEventsBLT()<<std::endl;
    SetAcquisitionMode("SW_CONTROLLED");
    SetExtTriggerInputMode(m_ExtTriggerMode);
    std::cout<<GetExtTriggerInputMode()<<std::endl;
    if(m_FamilyCode == "XX740" || m_FamilyCode == "XX742")
    {
      SetGroupEnableMask(m_EnableMask);
      for(std::size_t i = 0; i < (m_Nch / 8); i++)
      {
        if(m_EnableMask & (1 << i))
        {
          if(m_FamilyCode == "XX742")
          {
            for(std::size_t j = 0; j < 8; j++)
            {
              if(m_DCoffsetGrpCh[i][j] != -1)
                SetChannelDCOffset((i * 8) + j, m_DCoffsetGrpCh[i][j]);
              else
                SetChannelDCOffset((i * 8) + j, m_DCoffset[i]);
            }
          }
          else
          {
            if(m_Version_used[i] == 1) Set_calibrated_DCO(i);
            else SetGroupDCOffset(i, m_DCoffset[i]);
            SetGroupSelfTrigger(m_ChannelTriggerMode[i], (1 << i));
            SetGroupTriggerThreshold(i, m_Threshold[i]);
            SetChannelGroupMask(i, m_GroupTrgEnableMask[i]);
          }
          SetTriggerPolarity(i, m_PulsePolarity[i]);  //.TriggerEdge
        }
      }
    }
    else
    {
      SetChannelEnableMask(m_EnableMask);
      for(std::size_t i = 0; i < m_Nch; i++)
      {
        if(m_EnableMask & (1 << i))
        {
          if(m_Version_used[i] == 1) Set_calibrated_DCO(i);
          else SetChannelDCOffset(i, m_DCoffset[i]);
          if(m_FamilyCode != "XX730" && m_FamilyCode != "XX725") SetChannelSelfTrigger(m_ChannelTriggerMode[i], (1 << i));
          SetChannelTriggerThreshold(i, m_Threshold[i]);
          SetTriggerPolarity(i, m_PulsePolarity[i]);  //.TriggerEdge
        }
      }
      if(m_FamilyCode == "XX730" || m_FamilyCode == "XX725")
      {
        // channel pair settings for x730 boards
        for(std::size_t i = 0; i < m_Nch; i += 2)
        {
          if(m_EnableMask & (0x3 << i))
          {
            std::string   mode        = m_ChannelTriggerMode[i];
            std::uint32_t pair_chmask = 0;

            // Build mode and relevant channelmask. The behaviour is that,
            // if the triggermode of one channel of the pair is DISABLED,
            // this channel doesn't take part to the trigger generation.
            // Otherwise, if both are different from DISABLED, the one of
            // the even channel is used.
            if(m_ChannelTriggerMode[i] != "DISABLED")
            {
              if(m_ChannelTriggerMode[i + 1] == "DISABLED")
                pair_chmask = (0x1 << i);
              else
                pair_chmask = (0x3 << i);
            }
            else
            {
              mode        = m_ChannelTriggerMode[i + 1];
              pair_chmask = (0x2 << i);
            }
            pair_chmask &= m_EnableMask;
            SetChannelSelfTrigger(mode, pair_chmask);
          }
        }
      }
    }
    if(m_FamilyCode == "XX742")
    {
      for(std::size_t i = 0; i < (m_Nch / 8); i++)
      {
        SetDRS4SamplingFrequency(m_DRS4Frequency);
        SetGroupFastTriggerDCOffset(i, m_FTDCoffset[i]);
        std::cout<<"Asked "<<m_FTDCoffset[i]<<" get "<<GetGroupFastTriggerDCOffset(i)<<std::endl;
        SetGroupFastTriggerThreshold(i, m_FTThreshold[i]);
        std::cout<<"Asked "<<m_FTThreshold[i]<<" get "<<GetGroupFastTriggerThreshold(i)<<std::endl;
      }
    }
    for(std::size_t i = 0; i < m_WriteRgisters.size(); i++) WriteRegisterBitmask(m_WriteRgisters[i][0],m_WriteRgisters[i][1],m_WriteRgisters[i][2]);
  }
  catch(const Exception& error)
  {
    sendError("Errors found during the programming of the digitizer !\n Some settings may not be executed");
    throw;
  }
}

std::uint32_t CAENDigitizerBoard::get_time()
{
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

bool CAENDigitizerBoard::BoardSupportsCalibration()
{
  if(m_FamilyCode == "XX761" || m_FamilyCode == "XX751" || m_FamilyCode == "XX730" || m_FamilyCode == "XX725") return true;
  else return false;
}

bool CAENDigitizerBoard::BoardSupportsTemperatureRead()
{
  if(m_FamilyCode == "XX751" || m_FamilyCode == "XX730" || m_FamilyCode == "XX725") return true;
  else return false;
}

void CAENDigitizerBoard::GetMoreBoardInfo()
{
  if(m_FamilyCode == "XX724" || m_FamilyCode == "XX781" || m_FamilyCode == "XX780")
  {
    m_Nbit = 14;
    m_Ts   = 1.0;
  }
  else if(m_FamilyCode == "XX720")
  {
    m_Nbit = 12;
    m_Ts   = 4.0;
  }
  else if(m_FamilyCode == "XX721")
  {
    m_Nbit = 8;
    m_Ts   = 2.0;
  }
  else if(m_FamilyCode == "XX731")
  {
    m_Nbit = 8;
    m_Ts   = 2.0;
  }
  else if(m_FamilyCode == "XX751")
  {
    m_Nbit = 10;
    m_Ts   = 1.0;
  }
  else if(m_FamilyCode == "XX761")
  {
    m_Nbit = 10;
    m_Ts   = 0.25;
  }
  else if(m_FamilyCode == "XX740")
  {
    m_Nbit = 12;
    m_Ts   = 16.0;
  }
  else if(m_FamilyCode == "XX725")
  {
    m_Nbit = 14;
    m_Ts   = 4.0;
  }
  else if(m_FamilyCode == "XX730")
  {
    m_Nbit = 14;
    m_Ts   = 2.0;
  }
  else if(m_FamilyCode == "XX742")
  {
    m_Nbit                = 12;
    std::string frequency = GetDRS4SamplingFrequency();
    if(frequency == "1GHz") m_Ts = 1.0;
    else if(frequency == "2.5GHz") m_Ts = 0.4;
    else if(frequency == "5GHz") m_Ts = 0.2;
    else if(frequency == "750MHz") m_Ts = (1.0 / 750.0) * 1000.0;
    if(m_FormFactor == "VME64" || m_FormFactor == "VME64X") m_MaxGroupNumber = 4;
    else if(m_FormFactor == "NIM" || m_FormFactor == "DESKTOP") m_MaxGroupNumber = 2;
  }
  else if((m_FamilyCode == "XX751" || m_FamilyCode == "XX731") && m_DesMode == "ENABLE")
  {
    m_Ts /= 2;
  }
  if(m_FamilyCode == "XX724" || m_FamilyCode == "XX781" ||
     m_FamilyCode == "XX780" || m_FamilyCode == "XX720" ||
     m_FamilyCode == "XX721" || m_FamilyCode == "XX751" ||
     m_FamilyCode == "XX761" || m_FamilyCode == "XX731")
  {
    if(m_FormFactor == "VME64" || m_FormFactor == "VME64X") m_Nch = 8;
    else if(m_FormFactor == "NIM" || m_FormFactor == "DESKTOP") m_Nch = 4;
  }
  else if(m_FamilyCode == "XX725" || m_FamilyCode == "XX730")
  {
    if(m_FormFactor == "VME64" || m_FormFactor == "VME64X") m_Nch = 16;
    else if(m_FormFactor == "NIM" || m_FormFactor == "DESKTOP") m_Nch = 8;
  }
  else if(m_FamilyCode == "XX740")
  {
    if(m_FormFactor == "VME64" || m_FormFactor == "VME64X") m_Nch = 64;
    else if(m_FormFactor == "NIM" || m_FormFactor == "DESKTOP") m_Nch = 32;
  }
  else if(m_FamilyCode == "XX742")
  {
    if(m_FormFactor == "VME64" || m_FormFactor == "VME64X") m_Nch = 36;
    else if(m_FormFactor == "NIM" || m_FormFactor == "DESKTOP") m_Nch = 16;
  }
}


CAENDigitizerBoard::~CAENDigitizerBoard()
{
  FreeReadoutBuffer();
  FreeEvent();
}


bool CAENDigitizerBoard::Interrupt()
{
  int32_t boardId;
  uint8_t     InterruptMask = (1 << m_VME_INTERRUPT_LEVEL);
  bool    doInterruptTimeout{false};
  int VMEHandle{-1};
  try
  {
    if(m_FormFactor == "VME64" || m_FormFactor == "VME64X") VMEHandle=VMEIRQWait(getName(),InterruptMask,m_INTERRUPT_TIMEOUT);
    else IRQWait(m_INTERRUPT_TIMEOUT);
  }
  catch(const Exception& exception)
  {
    if(exception.getCode()==CAEN_DGTZ_Timeout)
    {
      doInterruptTimeout = true;
      return doInterruptTimeout;
    }
    else throw;
  }
  try
  {
    if(m_FormFactor == "VME64" || m_FormFactor == "VME64X")
    {
      boardId=VMEIACKCycle(VMEHandle, m_VME_INTERRUPT_LEVEL);
    }
  }
  catch(const Exception& exception)
  {
    if((exception.getCode()!=CAEN_DGTZ_Success)||(boardId != m_VME_INTERRUPT_STATUS_ID))
    {
      doInterruptTimeout = true;
      return doInterruptTimeout;
    }
    else if(CAEN_DGTZ_IRQ_MODE_ROAK == CAEN_DGTZ_IRQ_MODE_ROAK) RearmInterrupt();
  }
  return doInterruptTimeout;
}



}  // namespace CAEN
