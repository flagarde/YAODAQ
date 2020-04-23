#include "CAENDigitizerException.hpp"

#include "CAENDigitizerType.h"

#include "magic_enum.hpp"

namespace CAEN
{
const char* CAENDigitizerException::errorStrings(const int& code)
{
  switch(code)
  {
    case CAEN_DGTZ_Success:
      return "Operation completed successfully";
    case CAEN_DGTZ_CommError:
      return "Communication error";
    case CAEN_DGTZ_GenericError:
      return "Unspecified error";
    case CAEN_DGTZ_InvalidParam:
      return "Invalid parameter";
    case CAEN_DGTZ_InvalidLinkType:
      return "Invalid Link Type";
    case CAEN_DGTZ_InvalidHandle:
      return "Invalid device handle";
    case CAEN_DGTZ_MaxDevicesError:
      return "Maximum number of devices exceeded";
    case CAEN_DGTZ_BadBoardType:
      return "The operation is not allowed on this type of board";
    case CAEN_DGTZ_BadInterruptLev:
      return "The interrupt level is not allowed";
    case CAEN_DGTZ_BadEventNumber:
      return "The event number is bad";
    case CAEN_DGTZ_ReadDeviceRegisterFail:
      return "Unable to read the registry";
    case CAEN_DGTZ_WriteDeviceRegisterFail:
      return "Unable to write into the registry";
    case CAEN_DGTZ_InvalidChannelNumber:
      return "The channel number is invalid";
    case CAEN_DGTZ_ChannelBusy:
      return "The Channel is busy";
    case CAEN_DGTZ_FPIOModeInvalid:
      return "Invalid FPIO Mode";
    case CAEN_DGTZ_WrongAcqMode:
      return "Wrong acquisition mode";
    case CAEN_DGTZ_FunctionNotAllowed:
      return "This function is not allowed for this module";
    case CAEN_DGTZ_Timeout:
      return "Communication Timeout";
    case CAEN_DGTZ_InvalidBuffer:
      return "The buffer is invalid";
    case CAEN_DGTZ_EventNotFound:
      return "The event is not found";
    case CAEN_DGTZ_InvalidEvent:
      return "The vent is invalid";
    case CAEN_DGTZ_OutOfMemory:
      return "Out of memory";
    case CAEN_DGTZ_CalibrationError:
      return "Unable to calibrate the board";
    case CAEN_DGTZ_DigitizerNotFound:
      return "Unable to open the digitizer";
    case CAEN_DGTZ_DigitizerAlreadyOpen:
      return "The Digitizer is already open";
    case CAEN_DGTZ_DigitizerNotReady:
      return "The Digitizer is not ready to operate";
    case CAEN_DGTZ_InterruptNotConfigured:
      return "The Digitizer has not the IRQ configured";
    case CAEN_DGTZ_DigitizerMemoryCorrupted:
      return "The digitizer flash memory is corrupted";
    case CAEN_DGTZ_DPPFirmwareNotSupported:
      return "The digitizer dpp firmware is not supported in this lib version";
    case CAEN_DGTZ_InvalidLicense:
      return "Invalid Firmware License";
    case CAEN_DGTZ_InvalidDigitizerStatus:
      return "The digitizer is found in a corrupted status";
    case CAEN_DGTZ_UnsupportedTrace:
      return "The given trace is not supported by the digitizer";
    case CAEN_DGTZ_InvalidProbe:
      return "The given probe is not supported for the given digitizer's trace";
    case CAEN_DGTZ_UnsupportedBaseAddress:
      return "The Base Address is not supported, it's a Desktop device?";
    case CAEN_DGTZ_NotYetImplemented:
      return "The function is not yet implemented";
  }
  throw Exception(STATUS_CODE_INVALID_PARAMETER);
}

std::string CAENDigitizerException::toString() const
{
  return std::string(
      magic_enum::enum_name(magic_enum::enum_cast<CAEN_DGTZ_ErrorCode>(getCode()).value()));
}

#if experimental_have_source_location == 1
CAENDigitizerException::CAENDigitizerException(const int&                         code,
                                       std::experimental::source_location loc)
    : Exception(code, errorStrings(code), loc)
{
  if(code != CAEN_DGTZ_Success) throw;
};
#elif have_source_location == 1
CAENDigitizerException::CAENDigitizerException(const int&           code,
                                       std::source_location loc)
    : Exception(code, errorStrings(code), loc)
{
  if(code != CAEN_DGTZ_Success) throw;
};
#else
CAENDigitizerException::CAENDigitizerException(const int& code)
    : Exception(code, errorStrings(code))
{
  if(code != CAEN_DGTZ_Success) throw;
};
#endif

}  // namespace CAEN
