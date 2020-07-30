#include "CAENFlashException.hpp"

namespace CAEN
{
const char* CAENFlashException::errorStrings(const int_least32_t& code)
{
  switch(code)
  {
    case SUCCESS:
      return "Success";
    case ACCESS_FAILED:
      return "Access Failed";
    case PARAMETER_NOT_ALLOWED:
      return "Parameter not allowed";
    case FILE_OPEN_ERROR:
      return "File open error";
    case MALLOC_ERROR:
      return "Malloc error";
    case UNINITIALIZED:
      return "Uninitialized";
    case NOT_IMPLEMENTED:
      return "Not implemented";
    case UNSUPPORTED_FLASH_DEVICE:
      return "Unsupported flash device";
    default:
      return "Error code unknown";
  }
}

CAENFlashException::CAENFlashException(const int_least32_t& code, const SourceLocation& location): Exception(code, errorStrings(code), location)
{
  if(code != SUCCESS) throw *this;
};

}  // namespace CAEN
