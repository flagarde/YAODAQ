#pragma once

#include "Board.hpp"
#include "Flash.hpp"

#include <memory>
#include <variant>

namespace CAEN
{
class CAEN_DGTZ_UINT16_EVENT_t;
class CAEN_DGTZ_UINT8_EVENT_t;
class CAEN_DGTZ_X742_EVENT_t;
class CAEN_DGTZ_X743_EVENT_t;

class CAEN_DGTZ_DPP_PHA_Event_t;
class CAEN_DGTZ_DPP_PSD_Event_t;
class CAEN_DGTZ_DPP_CI_Event_t;
class CAEN_DGTZ_DPP_QDC_Event_t;
class CAEN_DGTZ_751_ZLE_Event_t;
class CAEN_DGTZ_730_ZLE_Event_t;
class CAEN_DGTZ_DPP_X743_Event_t;
class CAEN_DGTZ_730_DAW_Event_t;

class EventInfo;

class DPPAcquisitionMode
{
public:
  DPPAcquisitionMode(const std::string& mode, const std::string& param);
  DPPAcquisitionMode() {}
  void        setAcqMode(const std::string& mode);
  void        setSaveParam(const std::string& param);
  std::string getAcqMode();
  std::string getSaveParam();

private:
  std::string AcqMode{""};
  std::string SaveParam{""};
};

class CAENDigitizerBoard: public Board
{
public:
  CAENDigitizerBoard(const std::string& name);

  /**************************************************************************/ /**
                                                                                * \fn          WriteRegister(const std::uint32_t& Address,const  std::uint32_t& Data);
                                                                                * \brief     Writes a 32-bit word in a specific address offset of the digitizer
                                                                                * \param    [IN] Address : the register address offset
                                                                                * \param    [IN] Data    : the 32-bit data to write
                                                                                ******************************************************************************/
  void WriteRegister(const std::uint32_t& Address, const std::uint32_t& Data);

  /**************************************************************************/ /**
                                                                                * \fn        ReadRegister(const std::uint32_t& Address,std::uint32_t& Data);
                                                                                * \brief     Reads a 32-bit word from a specific address offset of the digitizer
                                                                                * \param    [IN] Address : the register address offset
                                                                                * \return  Data    : the 32-bit data read from the digitizer
                                                                                ******************************************************************************/
  std::uint32_t ReadRegister(const std::uint32_t& Address);

  /**************************************************************************/ /**
                                                                                * \fn        void GetInfo()
                                                                                * \brief     Retrieves the board information of the digitizer
                                                                                ******************************************************************************/
  void GetInfo();

  /**************************************************************************/ /**
                                                                                * \fn        void Reset();
                                                                                * \brief     Resets the Digitizer. All internal registers and states are restored
                                                                                *           to defaults.
                                                                                ******************************************************************************/
  void Reset();

  /**************************************************************************/ /**
                                                                                * \fn        void ClearData();
                                                                                * \brief     Clears the data stored in the buffers of the Digitizer
                                                                                ******************************************************************************/
  void ClearData();

  /**************************************************************************/ /**
                                                                                * \fn        void SendSWtrigger();
                                                                                * \brief     Sends a Software trigger to the Digitizer
                                                                                ******************************************************************************/
  void SendSWtrigger();

  /**************************************************************************/ /**
                                                                                * \fn        void SWStartAcquisition();
                                                                                * \brief     Starts Digitizers acquisition.
                                                                                ******************************************************************************/
  void SWStartAcquisition();

  /**************************************************************************/ /**
                                                                                * \fn        void SWStopAcquisition();
                                                                                * \brief     Stops Digitizer acquisition
                                                                                ******************************************************************************/
  void SWStopAcquisition();

  /**************************************************************************/ /**
                                                                                * \fn          CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetInterruptConfig(int handle, CAEN_DGTZ_EnaDis_t state, uint8_t level, uint32_t status_id, uint16_t event_number, CAEN_DGTZ_IRQMode_t mode)
                                                                                * \brief     Enable/disable Interrupts and set the Interrupt level used by the Digitizer
                                                                                * \param     [IN] state        : interrupts enable status
                                                                                * \param     [IN] level        : VME IRQ Level
                                                                                * \param     [IN] status_id    : VME status_id to assign to the Digitizer
                                                                                * \param     [IN] event_number : number of event required to raise interrupt
                                                                                * \param     [IN] mode         : interrupt mode [CAEN_DGTZ_IRQ_MODE_RORA|CAEN_DGTZ_IRQ_MODE_ROAK]
                                                                                ******************************************************************************/
  void SetInterruptConfig(const std::string& state, const std::uint8_t& level,
                          const std::uint32_t& status_id,
                          const std::uint16_t& event_number,
                          const std::string&   mode);

  /**************************************************************************/ /**
                                                                                * \fn          CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetInterruptConfig(int handle, CAEN_DGTZ_EnaDis_t *state, uint8_t *level, uint32_t *status_id, uint16_t *event_number, CAEN_DGTZ_IRQMode_t *mode)
                                                                                * \brief     Gets current Interrupt settings of the Digitizer
                                                                                * \param     [OUT] state       : interrupts state [CAEN_DGTZ_ENABLE|CAEN_DGTZ_DISABLE]
                                                                                * \param     [OUT] level       : current VME IRQ Level of the Digitizer
                                                                                * \param     [OUT] status_id   : current VME status_id of the the Digitizer
                                                                                * \param     [OUT] event_number: number of event required to raise an interrupt
                                                                                * \param     [OUT] mode        : interrupt mode [CAEN_DGTZ_IRQ_MODE_RORA|CAEN_DGTZ_IRQ_MODE_ROAK]
                                                                                * \return  0 = Success; negative numbers are error codes
                                                                                ******************************************************************************/
  // CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetInterruptConfig(int handle,
  // CAEN_DGTZ_EnaDis_t *state, uint8_t *level, uint32_t *status_id, uint16_t
  // *event_number, CAEN_DGTZ_IRQMode_t *mode);

  /**************************************************************************/ /**
                                                                                * \fn        void IRQWait(const std::uint32_t& timeout)
                                                                                * \brief     Waits for an interrupt by the Digitizer
                                                                                * \param     [IN] handle  : digitizer handle
                                                                                * \param     [IN] timeout : timeout (in milliseconds)
                                                                                ******************************************************************************/
  void IRQWait(const uint32_t& timeout);

  /**************************************************************************/ /**
                                                                                * \brief     Waits for an interrupt from a CAEN VME Bridge
                                                                                *
                                                                                * \param   [IN]  LinkType  : The link used to connect to the CAEN VME Bridge
                                                                                * \param   [IN]  LinkNum   :
                                                                                *                            - when using CONET, it is the optical link number to be used
                                                                                *                            - when using USB, it is the USB device number to be used
                                                                                * \param   [IN]  ConetNode :
                                                                                *                            - for CONET identify  witch device in the daisy-chain is addressed
                                                                                *                            - for USB must be 0.
                                                                                * \param   [IN]  IRQMask   : A bit-mask indicating the IRQ lines
                                                                                * \param   [IN]     timeout   : timeout (in milliseconds)
                                                                                * \param   [OUT] VMEHandle : device handler of the CAEN VME Bridge that received interrupt request
                                                                                * \return  0 = Success; negative numbers are error codes
                                                                                ******************************************************************************/
  // CAEN_DGTZ_ErrorCode CAENDGTZ_API
  // CAEN_DGTZ_VMEIRQWait(CAEN_DGTZ_ConnectionType LinkType, int LinkNum, int
  // ConetNode, uint8_t IRQMask, uint32_t timeout, int *VMEHandle);

  /**************************************************************************/ /**
                                                                                * \brief     Checks VME interrupt level
                                                                                *
                                                                                * \param   [IN] VMEhandle : handle of CAEN VME Bridge that raised the interrupt request (retrieved from CAEN_DGTZ_VMEIRQWait function)
                                                                                * \param   [OUT] Mask     : bitmask representing the active VME interrupt requests.
                                                                                * \return  0 = Success; negative numbers are error codes
                                                                                ******************************************************************************/
  // CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_VMEIRQCheck(int VMEHandle,
  // uint8_t *Mask);

  /**************************************************************************/ /**
                                                                                * \brief     Performs a VME Interrupt Acknowledge cycle to know the board_id of the board that raised an interrupt
                                                                                *
                                                                                * \param   [IN] handle     : handle of the CAEN VME Bridge that raised the interrupt (retrieved from CAEN_DGTZ_VMEIRQWait function)
                                                                                * \param   [IN] level      : VME interrupt level to acknowledge
                                                                                * \param   [OUT] board_id  : VME Digitizer board id of the interrupter
                                                                                * \return  0 = Success; negative numbers are error codes
                                                                                ******************************************************************************/
  // CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_VMEIACKCycle(int VMEHandle,
  // uint8_t level, int32_t *board_id);

  /**************************************************************************/ /**
                                                                                * \brief     Sets Dual Edge Sampling (DES) mode. Valid only for digitizers that supports this acquisiton mode
                                                                                * \param     [IN] enable : Enable/Disable Dual Edge Sampling mode
                                                                                ******************************************************************************/
  void SetDESMode(const std::string& enable);

  /**************************************************************************/ /**
                                                                                * \brief     Gets Dual Edge Sampling (DES) mode. Valid only for digitizers that supports this acquisiton mode.
                                                                                * \param     [OUT] enable : shows current DES mode status (enabled/disabled)
                                                                                ******************************************************************************/
  std::string GetDESMode();

  /**************************************************************************/ /**
                                                                                * \brief     Sets acquisition record length
                                                                                * \param     [IN] size    : the size of the record (in samples)
                                                                                * \param    [IN] channel : \c int specifying the channel to set the record length for.
                                                                                *                           DPP-PSD and DPP-CI require this parameter,
                                                                                *                           DPP-PHA ignores it
                                                                                ******************************************************************************/
  void SetRecordLength(const std::uint32_t& size = 0, const int& ch = -1);

  /**************************************************************************/ /**
                                                                                * \brief     Gets current acquisition record length
                                                                                * \param     [OUT] size   : the size of the record (in samples)
                                                                                * \param    [IN] channel : \c int specifying the channel to get the record length for.
                                                                                *                           DPP-PSD and DPP-CI require this parameter,
                                                                                *                           DPP-PHA ignores it
                                                                                * \return  0 = Success; negative numbers are error codes
                                                                                ******************************************************************************/
  std::uint32_t GetRecordLength(const int& ch = -1);

  /**************************************************************************/ /**
                                                                                * \brief     Sets channels that will be  enabled into events
                                                                                * \param     [IN] mask   : enabled channels mask.
                                                                                ******************************************************************************/
  void SetChannelEnableMask(const std::uint32_t& mask);

  /**************************************************************************/ /**
                                                                                * \brief     Gets current mask of enabled channels in events.
                                                                                *           If a mask bit is set, corresponding channel is currenly enabled for event readout
                                                                                * \return  mask  : enabled channels mask.
                                                                                ******************************************************************************/
  std::uint32_t GetChannelEnableMask();

  /**************************************************************************/ /**
                                                                                * \brief     Sets channels group that will be enabled into events.
                                                                                *           Valid only for digitizers that supports channel groups (V1740, DT5740 for instance).
                                                                                *           Please refer to digitizers documentation.
                                                                                * \param     [IN] mask   : channels group mask.
                                                                                ******************************************************************************/
  void SetGroupEnableMask(const std::uint32_t& mask);

  /**************************************************************************/ /**
                                                                                * \brief     Gets current mask of enabled channel groups in events.
                                                                                *           Valid only for digitizers that supports channel groups (V1740, DT5740 for instance).
                                                                                * \return  mask  : current channels group mask.
                                                                                ******************************************************************************/
  std::uint32_t GetGroupEnableMask();

  /**************************************************************************/ /**
                                                                                * \brief     Sets oneof the available trigger mode
                                                                                * \param     [IN] mode   : trigger mode
                                                                                ******************************************************************************/
  void SetSWTriggerMode(const std::string& mode);

  /**************************************************************************/ /**
                                                                                * \brief     Sets external trigger input mode
                                                                                * \param     [IN] mode   : external trigger input mode.
                                                                                ******************************************************************************/
  void SetExtTriggerInputMode(const std::string& mode);

  /**************************************************************************/ /**
                                                                                * \brief     Gets current external trigger input mode
                                                                                * \return  mode  : extrnal input trigger mode.
                                                                                ******************************************************************************/
  std::string GetExtTriggerInputMode();

  /**************************************************************************/ /**
                                                                                * \brief     Sets software trigger mode
                                                                                * \return  mode  : trigger mode
                                                                                ******************************************************************************/
  std::string GetSWTriggerMode();

  /**************************************************************************/ /**
                                                                                * \brief     Sets channel self trigger mode according to CAEN_DGTZ_TriggerMode_t.
                                                                                * \param     [IN] mode        : self trigger mode
                                                                                * \param    [IN] channelmask : channel mask to select affected channels.
                                                                                * NOTE: since x730 board family has even and odd channels paired, the user
                                                                                *  shouldn't call this function separately for the channels of the same pair,
                                                                                *  otherwise the second call will overwrite the setting of the first one. The
                                                                                *  user should instead call at maximum once for every pair with the relevant
                                                                                *  bits of the channelmask already set to the correct value.
                                                                                ******************************************************************************/
  void SetChannelSelfTrigger(const std::string&   mode,
                             const std::uint32_t& channelmask);

  /**************************************************************************/ /**
                                                                                * \brief     Gets current channel self trigger mode setting.
                                                                                * \param    [IN] channel : select channel to get the self trigger mode
                                                                                * \return  mode   : current trigger mode for selected channel
                                                                                ******************************************************************************/
  std::string GetChannelSelfTrigger(const std::uint32_t& channel);

  /**************************************************************************/ /**
                                                                                * \brief     Sets channel group self trigger mode according to mode
                                                                                *           Valid only for digitizers that supports channel groups (V1740, DT5740 for instance).
                                                                                * \param     [IN] mode      : trigger mode to set for selected channel groups
                                                                                * \param    [IN] groupmask : channel group selection mask
                                                                                ******************************************************************************/
  void SetGroupSelfTrigger(const std::string& mode, const std::uint32_t& group);

  /**************************************************************************/ /**
                                                                                * \brief     Gets channel group self trigger mode.
                                                                                *           Valid only for digitizers that supports channel groups (V1740, DT5740 for instance).
                                                                                * \param    [IN] group  : the group of channels to get the trigger information
                                                                                * \return  mode  : the mode of the trigger
                                                                                ******************************************************************************/
  std::string GetGroupSelfTrigger(const std::uint32_t& group);

  /**************************************************************************/ /**
                                                                                * \brief     Selects which channel is enabled to contribute to event among available channels of selected channel group.
                                                                                *           Valid only for digitizers that supports channel groups (V1740, DT5740 for instance).
                                                                                * \param     [IN] group       : channel group.
                                                                                * \param     [IN] channelmask : mask of channels to enable in event readout
                                                                                ******************************************************************************/
  void SetChannelGroupMask(const std::uint32_t  group,
                           const std::uint32_t& channelmask);

  /**************************************************************************/ /**
                                                                                * \brief     Gets current channel that are enabled to contribute to event among available channels of selected channel group.
                                                                                *           Valid only for digitizers that supports channel groups (V1740, DT5740 for instance).
                                                                                * \param     [IN] group        : the group of channels.
                                                                                * \return  channelmask : mask of channels to enable in event readout
                                                                                ******************************************************************************/
  std::uint32_t GetChannelGroupMask(const std::uint32_t& group);

  /**************************************************************************/ /**
                                                                                * \brief     Sets post trigger for next acquisitions
                                                                                * \param     [IN] percent : the percent of current record length
                                                                                ******************************************************************************/
  void SetPostTriggerSize(const std::uint32_t& percent);

  /**************************************************************************/ /**
                                                                                * \brief     Gets current post trigger length
                                                                                * \return  percent : the percent of the record
                                                                                ******************************************************************************/
  std::uint32_t GetPostTriggerSize();

  /**************************************************************************/ /**
   * \brief     Sets the pre-trigger size, which is the portion of acquisition
   window visible before a trigger
   * \note        This function is only available to DPP enabled firmwares and
   only to DPP-PHA, DPP-PSD and DPP-CI
   * \param     [IN] ch      : the channel to set the pre-trigger for. ch=-1
   writes the value for all channels. DPP-CI only supports ch=-1 (different
   channels must have the same pre-trigger)
   * \param     [IN] sample  : the pre-trigger size, in samples
   ******************************************************************************/
  void SetDPPPreTriggerSize(const int& ch, const std::uint32_t& samples);

  /**************************************************************************/ /**
                                                                                * \brief     Gets the pre-trigger size
                                                                                * \param     [IN] ch      : the channel to get the pre-trigger of
                                                                                * \return  sample : the pre-trigger size, in samples
                                                                                ******************************************************************************/
  std::uint32_t GetDPPPreTriggerSize(const int& ch);

  /**************************************************************************/ /**
                                                                                * \brief     Sets the DC offset for a specified channel
                                                                                * \param     [IN] channel : the channel to set
                                                                                * \param    [IN] Tvalue  : the DC offset to set. Tvalue is expressed in channel DAC (Digital to Analog Converter) steps.
                                                                                *                          Please refer to digitizer documentation for possible value range.
                                                                                ******************************************************************************/
  void SetChannelDCOffset(const std::uint32_t& channel,
                          const std::uint32_t& Tvalue);

  /**************************************************************************/ /**
                                                                                * \brief     Gets the DC offset for a specified channel
                                                                                * \param     [IN]  channel : the channel which takes the information.
                                                                                * \return  Tvalue  : the DC offset to set. Tvalue is expressed in channel DAC (Digital to Analog Converter) steps.
                                                                                ******************************************************************************/
  std::uint32_t GetChannelDCOffset(const std::uint32_t& channel);

  /**************************************************************************/ /**
                                                                                * \brief     Sets the DC offset for a specified group of channels
                                                                                * \param     [IN] group  : the group of channels to set
                                                                                * \param    [IN] Tvalue : the DC offset to set
                                                                                *                         Tvalue is expressed in channel DAC (Digital to Analog Converter) steps.
                                                                                *                         Please refer to digitizer documentation for possible value range.
                                                                                ******************************************************************************/
  void SetGroupDCOffset(const std::uint32_t& group,
                        const std::uint32_t& Tvalue);

  /**************************************************************************/ /**
                                                                                * \brief     Gets the DC offset from a specified group of channels
                                                                                * \param     [IN]  group  : the group of channels which takes the information.
                                                                                * \return  Tvalue : the DC offset set
                                                                                *                          Tvalue is expressed in channel DAC (Digital to Analog Converter) steps.
                                                                                *                          Please refer to digitizer documentation for possible value range.
                                                                                ******************************************************************************/
  std::uint32_t GetGroupDCOffset(const std::uint32_t& group);

  /**************************************************************************/ /**
                                                                                * \brief     Sets the Trigger Threshold for a specific channel
                                                                                * \param     [IN] channel : channel to set
                                                                                * \param    [IN] Tvalue  : threshold value to set
                                                                                ******************************************************************************/
  void SetChannelTriggerThreshold(const std::uint32_t& channel,
                                  const std::uint32_t& Tvalue);

  /**************************************************************************/ /**
                                                                                * \brief     Gets current Trigger Threshold from a specified channel
                                                                                * \param     [IN]  channel : the channel which takes the information.
                                                                                * \return  Tvalue  : the threshold value set
                                                                                ******************************************************************************/
  std::uint32_t GetChannelTriggerThreshold(const std::uint32_t& channel);

  /**************************************************************************/ /**
                                                                                * \brief     Set the pulse polarity for the specified channel
                                                                                * \param     [IN] channel : channel to set.
                                                                                * \param     [IN] pol     : the value of the pulse polarity
                                                                                ******************************************************************************/
  void SetChannelPulsePolarity(const std::uint32_t& channel,
                               const std::string&   pol);

  /**************************************************************************/ /**
                                                                                * \brief     Get the value of the pulse polarity for the specified channel
                                                                                * \param     [IN] channel : channel to get information from
                                                                                * \return  pol    : the value of the pulse polarity
                                                                                ******************************************************************************/
  std::string GetChannelPulsePolarity(const std::uint32_t& channel);

  /**************************************************************************/ /**
                                                                                * \brief     Sets the Trigger Threshold for a specified group of channels
                                                                                * \param     [IN] group  : the group of channels to set.
                                                                                * \param    [IN] Tvalue : the threshold value to set.
                                                                                ******************************************************************************/
  void SetGroupTriggerThreshold(const std::uint32_t& group,
                                const std::uint32_t& Tvalue);

  /**************************************************************************/ /**
                                                                                * \brief     Gets the Trigger Threshold from a specified group of channels
                                                                                * \param     [IN]  group  : the group of channels which takes the information.
                                                                                * \return  Tvalue : the threshold value to set.
                                                                                ******************************************************************************/
  std::uint32_t GetGroupTriggerThreshold(const std::uint32_t& group);

  /**************************************************************************/ /**
                                                                                * \brief     Sets Zero Suppression mode.
                                                                                * \param     [IN] mode   : Zero Suppression mode.
                                                                                ******************************************************************************/
  void SetZeroSuppressionMode(const std::string& mode);

  /**************************************************************************/ /**
                                                                                * \brief     Gets current Zero Suppression mode
                                                                                * \return  mode  : Zero Suppression mode
                                                                                ******************************************************************************/
  std::string GetZeroSuppressionMode();

  /**************************************************************************/ /**
                                                                                * \brief     Sets Zero Suppression parameters for a specific channel
                                                                                * \param     [IN] channel   : target channel
                                                                                * \param     [IN] weight    : Zero Suppression Weight
                                                                                * \param     [IN] threshold : Zero Suppression Threshold
                                                                                * \param     [IN] nsamp     : Number of samples to store before/after threshold crossing
                                                                                ******************************************************************************/
  // CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetChannelZSParams(int handle,
  // uint32_t channel, CAEN_DGTZ_ThresholdWeight_t weight, int32_t  threshold,
  // int32_t nsamp);

  /**************************************************************************/ /**
                                                                                * \brief     Gets current Zero Suppression parameters from a specified channel
                                                                                * \param     [IN]  channel   : target channel
                                                                                * \param     [OUT] weight    : Zero Suppression Weight
                                                                                * \param     [OUT] threshold : Zero Suppression Threshold
                                                                                * \param     [OUT] nsamp     : Number of samples to store before/after threshold crossing
                                                                                * \return  0 = Success; negative numbers are error codes
                                                                                ******************************************************************************/
  // CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetChannelZSParams(int handle,
  // uint32_t channel, CAEN_DGTZ_ThresholdWeight_t *weight, int32_t  *threshold,
  // int32_t *nsamp);

  /**************************************************************************/ /**
                                                                                * \brief     Sets digitizer acquisition mode
                                                                                * \param     [IN] mode   : acquisition mode
                                                                                ******************************************************************************/
  void SetAcquisitionMode(const std::string& mode);

  /**************************************************************************/ /**
                                                                                * \brief     Gets the acquisition mode of the digitizer
                                                                                * \return  mode   : the acquisition mode set
                                                                                ******************************************************************************/
  std::string GetAcquisitionMode();

  /**************************************************************************/ /**
                                                                                * \brief  Sets the run synchronization mode of the digitizer, used to synchronize an acquisition on multiple boards
                                                                                * \param     [IN] mode   : the run synchronization mode to set
                                                                                ******************************************************************************/
  void SetRunSynchronizationMode(const std::string& mode);

  /**************************************************************************/ /**
                                                                                * \brief   Gets the run synchronization mode of the digitizer
                                                                                * \return  mode   : the current run synchronization mode
                                                                                ******************************************************************************/
  std::string GetRunSynchronizationMode();

  /**************************************************************************/ /**
                                                                                * \brief     Sets waveform to output on Digitizer Analog Monitor Front Panel output
                                                                                * \param     [IN] mode   : Analog Monitor mode.
                                                                                ******************************************************************************/
  void SetAnalogMonOutput(const std::string& mode);

  /**************************************************************************/ /**
                                                                                * \brief     Gets current waveform selected to drive Digitizer Analog Monitor Front Panel output
                                                                                * \return  mode   : Analog Monitor output mode
                                                                                ******************************************************************************/
  std::string GetAnalogMonOutput();

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

  /**************************************************************************/ /**
                                                                                * \brief     Disables BERR as transfer termination signal from slave (digitizer)
                                                                                ******************************************************************************/
  void DisableEventAlignedReadout();

  /**************************************************************************/ /**
                                                                                * \brief     Enable or disable the Pack 2.5 mode of V1720/DT5720 Digitizers
                                                                                * \param     [IN] mode: Enable/Disable the Pack 2,5 mode
                                                                                ******************************************************************************/
  void SetEventPackaging(const std::string& on);

  /**************************************************************************/ /**
                                                                                * \brief     get the information about the Pack 2.5 mode of V1720/DT5720 Digitizers
                                                                                * \return  mode: Enable/Disable the Pack 2,5 mode
                                                                                ******************************************************************************/
  std::string GetEventPackaging();

  /**************************************************************************/ /**
                                                                                * \brief     Sets max aggregate number for each transfer
                                                                                * \On DPP-PHA, DPP-PSD and DPP-CI you can use CAEN_DGTZ_SetDPPEventAggregation
                                                                                * \param     [IN] numAggr : Maximum Event Number for transfer
                                                                                ******************************************************************************/
  void SetMaxNumAggregatesBLT(const std::uint32_t& numAggr);

  /**************************************************************************/ /**
                                                                                * \brief     Sets max event number for each transfer
                                                                                * \deprecated On DPP-PHA, DPP-PSD and DPP-CI use CAEN_DGTZ_SetDPPEventAggregation
                                                                                * \param     [IN] numEvents : Maximum Event Number for transfer
                                                                                ******************************************************************************/
  void SetMaxNumEventsBLT(const std::uint32_t& numEvents);

  /**************************************************************************/ /**
                                                                                * \brief     Gets the max number of aggregates of each block transfer
                                                                                * \return  numEvents : the number of aggregates set.
                                                                                ******************************************************************************/
  std::uint32_t GetMaxNumAggregatesBLT();

  /**************************************************************************/ /**
                                                                                * \brief     Gets the max number of events of each block transfer
                                                                                * \deprecated On DPP-PHA, DPP-PSD and DPP-CI use CAEN_DGTZ_SetDPPEventAggregation
                                                                                * \return  numEvents : the number of events set.
                                                                                ******************************************************************************/
  std::uint32_t GetMaxNumEventsBLT();

  /**************************************************************************/ /**
                                                                                * \brief     Allocates memory buffer to hold data received from digitizer.
                                                                                * \note     Grandfathered into the <b>new readout API</b>
                                                                                ******************************************************************************/
  void MallocReadoutBuffer();

  /**************************************************************************/ /**
                                                                                * \brief     Reads data (events) from the digitizer.
                                                                                * \note        Grandfathered into the <b>new readout API</b>
                                                                                ******************************************************************************/
  void ReadData(const std::string& mode = "SLAVE_TERMINATED_READOUT_MBLT");

  /**************************************************************************/ /**
                                                                                * \brief     Frees memory allocated by the CAEN_DGTZ_MallocReadoutBuffer.
                                                                                * \note     Grandfathered into the <b>new readout API</b>
                                                                                ******************************************************************************/
  void FreeReadoutBuffer();

  /**************************************************************************/ /**
                                                                                * \brief     Gets current number of event stored in the acquisition buffer.
                                                                                * \deprecated On DPP-PHA, DPP-PSD and DPP-CI use the <b>new readout API</b>
                                                                                * \return  numEvents : Number of events stored in the acquisition buffer
                                                                                ******************************************************************************/
  std::uint32_t GetNumEvents();

  /**************************************************************************/ /**
                                                                                * \brief     Retrieves the information associated with a specified event
                                                                                * \deprecated On DPP-PHA, DPP-PSD and DPP-CI use the <b>new readout API</b>
                                                                                * \param     [IN] numEvents  : Number of events stored in the acquisition buffer
                                                                                ******************************************************************************/
  EventInfo GetEventInfo(const std::int32_t& numEvent);

  /**************************************************************************/ /**
                                                                                * \brief     Decodes a specified event stored in the acquisition buffer
                                                                                * \deprecated On DPP-PHA, DPP-PSD and DPP-CI use the <b>new readout API</b>
                                                                                ******************************************************************************/
  void DecodeEvent();

  /**************************************************************************/ /**
                                                                                * \brief     Releases the event returned by the CAEN_DGTZ_DecodeEvent
                                                                                * \deprecated On DPP-PHA, DPP-PSD and DPP-CI use the <b>new readout API</b>
                                                                                ******************************************************************************/
  void FreeEvent();

  /**************************************************************************/ /**
                                                                                * \brief     Decodes and returns all the DPP events stored in the acquisition buffers.
                                                                                * \note     Part of the <b>new readout API</b>
                                                                                * \param     [IN]  buffer     : address of the acquisition buffer
                                                                                * \param     [IN]  buffsize   : acquisition buffer size (in samples)
                                                                                * \param     [OUT] events     : pointer to the event list (allocated via MallocDPPEvents)
                                                                                * \param    [OUT] numEventsArray  : pointer to an array of int which will contain the number of events found per channel
                                                                                * \return  0 = Success; negative numbers are error codes
                                                                                ******************************************************************************/
  // CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetDPPEvents(int handle, char
  // *buffer, uint32_t buffsize, void **events, uint32_t *numEventsArray);

  /**************************************************************************/ /**
                                                                                * \brief     Allocates the event buffer matrix which is handled by CAEN_DGTZ_GetDPPEvents.
                                                                                *          The matrix has one event array per channel and must be declared as a MAX_CH-sized array of pointers.
                                                                                * \note     Part of the <b>new readout API</b>
                                                                                ******************************************************************************/
  void MallocDPPEvents();

  /**************************************************************************/ /**
                                                                                * \brief     Deallocates the event buffer matrix.
                                                                                * \note     Part of the <b>new readout API</b>
                                                                                ******************************************************************************/
  void FreeDPPEvents();

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
   specify the channel (required for DPP-PSD and DPP-CI, ignored by DPP-PHA)
   * \return  0 = Success; negative numbers are error codes
   ******************************************************************************/
  // CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetNumEventsPerAggregate(int
  // handle, uint32_t numEvents, ...);

  /**************************************************************************/ /**
   * \brief     Gets the number of events that each aggregate will contain
   * \deprecated
   * \param     [OUT] numEvents  : number of events per aggregate
   * \param     [IN] channel     : optional parameter in the form of an int to
   specify the channel (required for DPP-PSD and DPP-CI, ignored by DPP-PHA)
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

  /**************************************************************************/ /**
                                                                                * \brief     Set the DPP acquisition mode.
                                                                                * \param     [IN] mode: The DPP acquisition mode
                                                                                * \param     [IN] param: The acquisition data to retrieve in acquisition
                                                                                ******************************************************************************/
  void SetDPPAcquisitionMode(DPPAcquisitionMode& acq);

  /**************************************************************************/ /**
                                                                                * \brief     Get the information about the DPP acquisition mode.
                                                                                * \param     [OUT] mode: The DPP acquisition mode
                                                                                * \param     [OUT] param: The acquisition data to retrieve in acquisition
                                                                                * \return  DPPAcquisitionMode
                                                                                ******************************************************************************/
  DPPAcquisitionMode GetDPPAcquisitionMode();

  /**************************************************************************/ /**
                                                                                * \brief     Sets the DPP Trigger mode (DPP-PSD and DPP-CI only)
                                                                                * \param     [IN] mode:   the desired trigger mode:
                                                                                *                         - CAEN_DGTZ_DPP_TriggerMode_Normal
                                                                                *                          - CAEN_DGTZ_DPP_TriggerMode_Coincidence
                                                                                ******************************************************************************/
  void SetDPPTriggerMode(const std::string& mode);

  /**************************************************************************/ /**
                                                                                * \brief     Gets the current DPP Trigger mode (DPP-PSD and DPP-CI only)
                                                                                * \return  mode:   the current trigger mode
                                                                                ******************************************************************************/
  std::string GetDPPTriggerMode();

  /*****************************************************************************/ /**
                                                                                   * \brief     Set the virtual probe to be displayed on the given trace
                                                                                   * \param     [IN] trace: The Trace to be affected
                                                                                   * \param     [IN] probe:  The Virtual Probe to be set on the given trace
                                                                                   ******************************************************************************/
  void SetDPP_VirtualProbe(const int& trace, const int& probe);

  /*****************************************************************************/ /**
                                                                                   * \brief     Get the virtual probe currently displayed on the given trace
                                                                                   * \param     [IN] trace: The Trace to be get the probe of
                                                                                   * \param     [OUT] probe: The Virtual Probe displayed on the given trace
                                                                                   * \return  0 = Success; negative numbers are error codes
                                                                                   ******************************************************************************/
  // CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetDPP_VirtualProbe(int handle,
  // int trace, int *probe);

  /*****************************************************************************/ /**
                                                                                   * \brief     Get the list of virtual probes supported on board's given trace
                                                                                   * \param     [IN] trace: The Trace to be get the probes list of
                                                                                   * \param     [OUT] probes: The list of Virtual Probes supported by the trace
                                                                                   *                          It Must be an array of length MAX_SUPPORTED_PROBES
                                                                                   * \param     [OUT] numProbes: The number of Probes supported by the trace.
                                                                                   * \return  probes: The list of Virtual Probes supported by the trace
                                                                                   ******************************************************************************/
  // CAEN_DGTZ_ErrorCode CAENDGTZ_API
  // CAEN_DGTZ_GetDPP_SupportedVirtualProbes(int handle, int trace, int
  // probes[], int *numProbes);

  /*****************************************************************************/ /**
                                                                                   * \brief     Get the name of the given virtual probe
                                                                                   * \param     [IN] probe: The Virtual Probe to get the name of
                                                                                   * \return  name: The name of the given probe
                                                                                   ******************************************************************************/
  std::string VirtualProbeName(const int& probe);

  /**************************************************************************/ /**
                                                                                * \brief     Allocate the memory for the event
                                                                                ******************************************************************************/
  void AllocateEvent();

  /**************************************************************************/ /**
                                                                                * \brief     Sets the IO Level
                                                                                * \param     [IN] level   : The level to set
                                                                                ******************************************************************************/
  void SetIOLevel(const std::string& level);

  /**************************************************************************/ /**
                                                                                * \brief     Gets the IO Level
                                                                                * \return  level  : The IO level of the digitizer
                                                                                ******************************************************************************/
  std::string GetIOLevel();

  /**************************************************************************/ /**
                                                                                * \brief     Sets the trigger polarity of a specified channel
                                                                                * \param    [IN] channel : select channel to set the trigger polarity
                                                                                * \param     [IN] Polarity   : The polarity to set
                                                                                ******************************************************************************/
  void SetTriggerPolarity(const std::uint32_t& channel,
                          const std::string&   Polarity);

  /**************************************************************************/ /**
                                                                                * \brief     Gets the trigger polarity of a specified channel
                                                                                * \param    [IN] channel : select channel to get the trigger polarity
                                                                                * \return  Polarity   : The polarity of the specified channel
                                                                                ******************************************************************************/
  std::string GetTriggerPolarity(const std::uint32_t& channel);

  /**************************************************************************/ /**
                                                                                * \brief     Rearm the Interrupt
                                                                                ******************************************************************************/
  void RearmInterrupt();

  /**************************************************************************/ /**
                                                                                * \brief     CAEN_DGTZ_SetDRS4SamplingFrequency
                                                                                ******************************************************************************/
  void        SetDRS4SamplingFrequency(const std::string& freq);
  std::string GetDRS4SamplingFrequency();
  /*
  CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetOutputSignalMode(int handle,
  CAEN_DGTZ_OutputSignalMode_t mode); CAEN_DGTZ_ErrorCode CAENDGTZ_API
  CAEN_DGTZ_GetOutputSignalMode(int handle, CAEN_DGTZ_OutputSignalMode_t *mode);
  */
  void          SetGroupFastTriggerThreshold(const std::uint32_t& group,
                                             const std::uint32_t& Tvalue);
  std::uint32_t GetGroupFastTriggerThreshold(const std::uint32_t& group);
  void          SetGroupFastTriggerDCOffset(const std::uint32_t& group,
                                            const std::uint32_t& DCvalue);
  std::uint32_t GetGroupFastTriggerDCOffset(const std::uint32_t& group);
  void          SetFastTriggerDigitizing(const std::string& on);
  std::string   GetFastTriggerDigitizing();
  void          SetFastTriggerMode(const std::string& mode);
  std::string   GetFastTriggerMode();
  /*
  CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_LoadDRS4CorrectionData(int handle,
  CAEN_DGTZ_DRS4Frequency_t frequency); CAEN_DGTZ_ErrorCode CAENDGTZ_API
  CAEN_DGTZ_GetCorrectionTables(int handle, int frequency, void *CTable);
  CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_EnableDRS4Correction(int handle);
  CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_DisableDRS4Correction(int handle);
  CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_DecodeZLEWaveforms(int handle, void
  *event, void *waveforms); CAEN_DGTZ_ErrorCode CAENDGTZ_API
  CAEN_DGTZ_FreeZLEWaveforms(int handle, void *waveforms); CAEN_DGTZ_ErrorCode
  CAENDGTZ_API CAEN_DGTZ_MallocZLEWaveforms(int handle, void **waveforms,
  uint32_t *allocatedSize); CAEN_DGTZ_ErrorCode CAENDGTZ_API
  CAEN_DGTZ_FreeZLEEvents(int handle, void **events); CAEN_DGTZ_ErrorCode
  CAENDGTZ_API CAEN_DGTZ_MallocZLEEvents(int handle, void **events, uint32_t
  *allocatedSize); CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetZLEEvents(int
  handle, char *buffer, uint32_t buffsize, void **events, uint32_t*
  numEventsArray); CAEN_DGTZ_ErrorCode CAENDGTZ_API
  CAEN_DGTZ_SetZLEParameters(int handle, uint32_t channelMask, void* params);
  CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetSAMCorrectionLevel(int handle,
  CAEN_DGTZ_SAM_CORRECTION_LEVEL_t *level); CAEN_DGTZ_ErrorCode CAENDGTZ_API
  CAEN_DGTZ_SetSAMCorrectionLevel(int handle, CAEN_DGTZ_SAM_CORRECTION_LEVEL_t
  level); CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_EnableSAMPulseGen(int
  handle, int channel, unsigned short  pulsePattern,
  CAEN_DGTZ_SAMPulseSourceType_t pulseSource); CAEN_DGTZ_ErrorCode CAENDGTZ_API
  CAEN_DGTZ_DisableSAMPulseGen(int handle, int channel); CAEN_DGTZ_ErrorCode
  CAENDGTZ_API CAEN_DGTZ_SetSAMPostTriggerSize(int handle, int SamIndex, uint8_t
  value); CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetSAMPostTriggerSize(int
  handle, int SamIndex, uint32_t *value); CAEN_DGTZ_ErrorCode CAENDGTZ_API
  CAEN_DGTZ_SetSAMSamplingFrequency(int handle, CAEN_DGTZ_SAMFrequency_t
  frequency); CAEN_DGTZ_ErrorCode CAENDGTZ_API
  CAEN_DGTZ_GetSAMSamplingFrequency(int handle, CAEN_DGTZ_SAMFrequency_t
  *frequency); CAEN_DGTZ_ErrorCode CAENDGTZ_API _CAEN_DGTZ_Read_EEPROM(int
  handle, int EEPROMIndex, unsigned short add, int nbOfBytes, unsigned char*
  buf); CAEN_DGTZ_ErrorCode CAENDGTZ_API _CAEN_DGTZ_Write_EEPROM(int handle, int
  EEPROMIndex, unsigned short add, int nbOfBytes, void* buf);
  CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_LoadSAMCorrectionData(int handle);
  CAEN_DGTZ_ErrorCode CAENDGTZ_API _CAEN_DGTZ_TriggerThreshold(int handle,
  CAEN_DGTZ_EnaDis_t endis); CAEN_DGTZ_ErrorCode CAENDGTZ_API
  CAEN_DGTZ_SendSAMPulse(int handle); CAEN_DGTZ_ErrorCode CAENDGTZ_API
  CAEN_DGTZ_SetSAMAcquisitionMode(int handle, CAEN_DGTZ_AcquisitionMode_t mode);
  CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetSAMAcquisitionMode(int handle,
  CAEN_DGTZ_AcquisitionMode_t *mode); CAEN_DGTZ_ErrorCode CAENDGTZ_API
  CAEN_DGTZ_SetTriggerLogic(int handle, CAEN_DGTZ_TrigerLogic_t logic,  uint32_t
  majorityLevel); CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetTriggerLogic(int
  handle, CAEN_DGTZ_TrigerLogic_t *logic, uint32_t *majorityLevel);
  CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetChannelPairTriggerLogic(int
  handle,  uint32_t channelA, uint32_t channelB, CAEN_DGTZ_TrigerLogic_t *logic,
  uint16_t *coincidenceWindow); CAEN_DGTZ_ErrorCode CAENDGTZ_API
  CAEN_DGTZ_SetChannelPairTriggerLogic(int handle,  uint32_t channelA, uint32_t
  channelB, CAEN_DGTZ_TrigerLogic_t logic, uint16_t coincidenceWindow);
  */
  void          SetDecimationFactor(const std::uint16_t& factor);
  std::uint16_t GetDecimationFactor();
  /*
  CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetSAMTriggerCountVetoParam(int
  handle, int channel, CAEN_DGTZ_EnaDis_t enable, uint32_t vetoWindow);
  CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetSAMTriggerCountVetoParam(int
  handle, int channel, CAEN_DGTZ_EnaDis_t *enable, uint32_t *vetoWindow);
  CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetSAMCorrectionData(int handle,
  void *STable);*/

  /**************************************************************************/ /**
                                                                                * \brief     Calibrate the board
                                                                                ******************************************************************************/
  void Calibrate();

  /**************************************************************************/ /**
                                                                                * \brief     Read the temperature in celsius for the given channel
                                                                                * \param     [IN] ch     : channel to read the temperature for
                                                                                ******************************************************************************/
  std::uint32_t ReadTemperature(const std::int32_t& ch);

  /**************************************************************************/ /**
                                                                                * \brief     Get the DPP firmware type
                                                                                * \return  firmware  : Firmware type
                                                                                ******************************************************************************/
  std::string GetDPPFirmwareType();

  /*! \fn      int ProgramDigitizer(int handle, WaveDumpConfig_t WDcfg)
   *   \brief   configure the digitizer according to the parameters read from
   *            the cofiguration file and saved in the WDcfg data structure
   */
  void ProgramDigitizer();

  /*! \fn void Set_calibrated_DCO(const int &ch)
   * \brief   sets the calibrated DAC value using calibration data (only if
   * BASELINE_SHIFT is in use)
   */
  void Set_calibrated_DCO(const int& ch);

  /*! \fn      int WriteRegisterBitmask(int32_t handle, uint32_t address,
   * uint32_t data, uint32_t mask) \brief   writes 'data' on register at
   * 'address' using 'mask' as bitmask \param   address:   Address of the
   * Register to write \param   data   :   Data to Write on the Register \param
   * mask   :   Bitmask to use for data masking
   */
  void WriteRegisterBitmask(const std::uint32_t& address,
                            const std::uint32_t& data,
                            const std::uint32_t& mask);

  /*! \brief   return TRUE if board descriped by 'BoardInfo' supports
   *            calibration or not.
   */
  bool BoardSupportsCalibration();

  /*! \brief   return TRUE if board descriped by 'BoardInfo' supports
   *            temperature read or not.
   */
  bool BoardSupportsTemperatureRead();

  /*! \fn      void GetMoreBoardInfo()
   * \brief   calculate num of channels, num of bit and sampl period according
   * to the board type
   */
  void GetMoreBoardInfo();

  std::uint32_t get_time();

  virtual ~CAENDigitizerBoard()
  {
    std::cout << "Destroying CAENDigitizerBoard" << std::endl;
  }
  
  void  DoInitialize() final;
  
private:
  void initilizeParameters();

  virtual void verifyParameters() final;

  void        setModelName(const std::string& name);
  std::string m_ModelName{""};

  void        setModel(const std::uint32_t& model);
  std::string m_Model{""};

  void          setNbrChannels(const std::uint32_t& model);
  std::uint32_t m_NbrChannels{0};

  void        setFormFactor(const std::uint32_t& form);
  std::string m_FormFactor{""};

  void        setFamilyCode(const std::uint32_t& fam);
  std::string m_FamilyCode{""};

  void        setROCFirmwareRel(const std::string& firm);
  std::string m_ROC_FirmwareRel{""};

  void        setAMCFirmwareRel(const std::string& firm);
  std::string m_AMC_FirmwareRel{""};

  void          setSerialNumber(const std::uint32_t& model);
  std::uint32_t m_SerialNumber{0};

  void                       setMezzanineSerialNumber(char serials[4][8]);
  std::array<std::string, 4> m_MezzanineSerialNumber;

  void          setPCBRevision(const std::uint32_t& model);
  std::uint32_t m_PCB_Revision{0};

  void          setADCNBits(const std::uint32_t& model);
  std::uint32_t m_ADC_NBits{0};

  void          setSAMCorrectionDataLoaded(const std::uint32_t& model);
  std::uint32_t m_SAMCorrectionDataLoaded{0};

  void        setLicense(const std::string& name);
  std::string m_License{""};

  void          setCommHandle(const std::uint32_t& model);
  std::uint32_t m_CommHandle{0};

  void          setVMEHandle(const std::uint32_t& model);
  std::uint32_t m_VMEHandle{0};

  std::unique_ptr<char> m_Buffer{nullptr};
  std::uint32_t         m_AllocatedSize{0};
  std::uint32_t         m_AllocatedSizeDPP{0};
  std::uint32_t         m_BufferSize{0};
  std::unique_ptr<char> m_EventPtr{nullptr};

  CAEN_DGTZ_UINT16_EVENT_t* m_Uint16Event{nullptr};
  CAEN_DGTZ_UINT8_EVENT_t*  m_Uint8Event{nullptr};
  CAEN_DGTZ_X742_EVENT_t*   m_X742Event{nullptr};
  CAEN_DGTZ_X743_EVENT_t*   m_X743Event{nullptr};

  CAEN_DGTZ_DPP_PHA_Event_t*  m_DPPPHAEvent[16];
  CAEN_DGTZ_DPP_PSD_Event_t*  m_DPPPSDEvent[16];
  CAEN_DGTZ_DPP_CI_Event_t*   m_DPPCIEvent[16];
  CAEN_DGTZ_DPP_QDC_Event_t*  m_DPPQDCEvent[16];
  CAEN_DGTZ_751_ZLE_Event_t*  m_751ZLEEvent[8];
  CAEN_DGTZ_730_ZLE_Event_t*  m_730ZLEEvent[16];
  CAEN_DGTZ_DPP_X743_Event_t* m_DPPX743Event[2 * 8];
  CAEN_DGTZ_730_DAW_Event_t*  m_730DAWEvent[16];

  void Exit(const int& error);
  static constexpr int m_MAX_CH{64};/* max. number of channels */
  static constexpr int m_MAX_SET{16};/* max. number of independent settings */
  static constexpr int m_MAX_GROUPS{8};/* max. number of groups */
  bool                m_Test{false};
  std::string         m_FastTriggerEnabled{"DISABLED"};
  std::string         m_DesMode{"DISABLED"};
  std::uint32_t       m_RecordLength{1024};
  int                 m_NumberEvent{1023};
  std::uint16_t       m_DecimationFactor{1};
  int                 m_PostTrigger{50};
  std::string         m_FPIOtype{"NIM"};
  int                 m_InterruptNumEvents{0};
  const std::uint8_t  m_VME_INTERRUPT_LEVEL{1};
  const std::uint32_t m_VME_INTERRUPT_STATUS_ID{0xAAAA};
  int                 m_NumEvents{1023};
  std::string         m_ExtTriggerMode{"ACQ_ONLY"};
  std::uint16_t       m_EnableMask{0xFFFF};
  int                 m_Nch{0};
  int                 m_Nbit{0};
  float               m_Ts{0};
  bool                m_Header{true};
  std::uint32_t       m_MaxGroupNumber{0};
  std::string         m_FileFormat{"ROOT"};
  bool                m_StartupCalibration{false};
  //#define MAX_SET 16   /* max. number of independent settings */
  std::array<std::array<int32_t,m_MAX_SET>,m_MAX_SET> m_DCoffsetGrpCh;
  std::array<std::uint32_t,m_MAX_SET>           m_DCoffset;
  std::array<int,m_MAX_SET>                     m_Version_used;
  std::array<std::string,m_MAX_SET>             m_ChannelTriggerMode;
  std::array<std::uint32_t,m_MAX_SET>           m_Threshold;
  std::array<std::uint8_t,m_MAX_SET>            m_GroupTrgEnableMask;
  std::array<std::string,m_MAX_SET>             m_PulsePolarity;
  std::array<int, 64>                     m_DCfile;
  std::array<float,m_MAX_SET>                   m_Cal;
  std::array<float,m_MAX_SET>                   m_Offset;
  std::string                             m_DRS4Frequency{"5GHz"};
  std::array<std::uint32_t,m_MAX_SET>           m_FTDCoffset;
  std::array<std::uint32_t,m_MAX_SET>           m_FTThreshold;
  std::vector<std::array<uint32_t,3>> m_WriteRgisters;
  static std::vector<std::string>         ErrMsg;
  std::array<std::string, 4>              m_TablesFilenames;
  bool                                    m_UseCorrections{false};
  bool                                    m_UseManualTables{false};
  Flash                                   m_Flash;
  /* Error messages */
  typedef enum
  {
    ERR_NONE = 0,
    ERR_CONF_FILE_NOT_FOUND,
    ERR_DGZ_OPEN,
    ERR_BOARD_INFO_READ,
    ERR_INVALID_BOARD_TYPE,
    ERR_DGZ_PROGRAM,
    ERR_MALLOC,
    ERR_INTERRUPT,
    ERR_READOUT,
    ERR_EVENT_BUILD,
    ERR_UNHANDLED_BOARD,
    ERR_OVERTEMP,

    ERR_DUMMY_LAST,
  } ERROR_CODES;
};

}  // namespace CAEN
