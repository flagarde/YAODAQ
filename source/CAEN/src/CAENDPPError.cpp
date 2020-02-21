#include "CAENDPPError.hpp"

#include "CAENDPPLibTypes.h"

#include <cstdint>

namespace CAEN {
const char *CAENDPPError::ErrorStrings(const std::int_least32_t &code) {
  switch (code) {
  case CAENDPP_RetCode_Ok:
    return "Success";
  case CAENDPP_RetCode_GenericError:
    return "Generic Error";
  case CAENDPP_RetCode_TooManyInstances:
    return "Too Many Instances";
  case CAENDPP_RetCode_ProcessFail:
    return "Process Fail";
  case CAENDPP_RetCode_ReadFail:
    return "Read Fail";
  case CAENDPP_RetCode_WriteFail:
    return "Write Fail";
  case CAENDPP_RetCode_BadMessage:
    return "Invalid Response";
  case CAENDPP_RetCode_InvalidHandle:
    return "Invalid library handle";
  case CAENDPP_RetCode_ConfigError:
    return "Configuration Error";
  case CAENDPP_RetCode_BoardInitFail:
    return "Board Init Failed";
  case CAENDPP_RetCode_TimeoutError:
    return "Timeout Error";
  case CAENDPP_RetCode_InvalidParameter:
    return "Invalid Parameter";
  case CAENDPP_RetCode_NotInWaveMode:
    return "Not in Waveforms Mode";
  case CAENDPP_RetCode_NotInHistoMode:
    return "Not in Histogram Mode";
  case CAENDPP_RetCode_NotInListMode:
    return "Not in List Mode";
  case CAENDPP_RetCode_NotYetImplemented:
    return "Not Yet Implemented";
  case CAENDPP_RetCode_BoardNotConfigured:
    return "Board Not Configured";
  case CAENDPP_RetCode_InvalidBoardIndex:
    return "Invalid board index";
  case CAENDPP_RetCode_InvalidChannelIndex:
    return "Invalid channel index";
  case CAENDPP_RetCode_UnsupportedFirmware:
    return "Invalid board firmware";
  case CAENDPP_RetCode_NoBoardsAdded:
    return "No board added";
  case CAENDPP_RetCode_AcquisitionRunning:
    return "Acquisition Status is not compliant with the function called";
  case CAENDPP_RetCode_OutOfMemory:
    return "OutOfMemory";
  case CAENDPP_RetCode_BoardChannelIndex:
    return "Invalid board channel index";
  case CAENDPP_RetCode_HistoAlloc:
    return "No valid histogram allocated";
  case CAENDPP_RetCode_OpenDumper:
    return "Error Opening the List Dumper";
  case CAENDPP_RetCode_BoardStart:
    return "Error Starting Acquisition for a Board";
  case CAENDPP_RetCode_ChannelEnable:
    return "The given channel is not enabled";
  case CAENDPP_RetCode_InvalidCommand:
    return "Invalid Command";
  case CAENDPP_RetCode_NumBins:
    return "Invalid Number of Bins";
  case CAENDPP_RetCode_HistoIndex:
    return "Invalid Hitogram Index";
  case CAENDPP_RetCode_UnsupportedFeature:
    return "The feature is not supported by the gve board/channel";
  case CAENDPP_RetCode_BadHistoState:
    return "The given histogram is an invalid state (ex. 'done' while it "
           "shouldn't)";
  case CAENDPP_RetCode_NoMoreHistograms:
    return "Cannot switch to ext histo, no more histograms available";
  case CAENDPP_RetCode_NotHVBoard:
    return "The selected board doesn't support HV Channels";
  case CAENDPP_RetCode_InvalidHVChannel:
    return "Invalid HV channel index";
  case CAENDPP_RetCode_SocketSend:
    return "Error Sending Message through Socket";
  case CAENDPP_RetCode_SocketReceive:
    return "Error Receiving Message from Socket";
  case CAENDPP_RetCode_BoardThread:
    return "Cannot get Board's acquisition thread";
  case CAENDPP_RetCode_DecodeWaveform:
    return "Cannot decode waveform from buffer";
  case CAENDPP_RetCode_OpenDigitizer:
    return "Error Opening the digitizer";
  case CAENDPP_RetCode_BoardModel:
    return "Requested a feature incompatible with board's Manufacture";
  case CAENDPP_RetCode_AutosetStatus:
    return "Autoset Status is not compliant with the requested feature";
  case CAENDPP_RetCode_Autoset:
    return "Autoset error looking for signal parameters";
  case CAENDPP_RetCode_Calibration:
    return "Calibration error";
  case CAENDPP_RetCode_EventRead:
    return "Event Read Error";
  }
  return "Unknown error";
}

#if experimental_have_source_location == 1
CAENDPPError::CAENDPPError(const int &code,
                           std::experimental::source_location loc)
    : Error(code, ErrorStrings(code), loc) {
  if (code != CAENDPP_RetCode_Ok)
    throw *this;
};
#elif have_source_location == 1
CAENDPPError::CAENDPPError(const int &code, std::source_location loc)
    : Error(code, ErrorStrings(code), loc) {
  if (code != CAENDPP_RetCode_Ok)
    throw *this;
};
#else
CAENDPPError::CAENDPPError(const int &code) : Error(code, ErrorStrings(code)) {
  if (code != CAENDPP_RetCode_Ok)
    throw *this;
};
#endif

} // namespace CAEN
