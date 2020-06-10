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
    default :
      return "Error code unknown";
  }
}

#if experimental_have_source_location == 1
CAENFlashException::CAENFlashException(const int_least32_t& code, const std::experimental::source_location& loc) : Exception(code, errorStrings(code), loc)
{
  if(code != SUCCESS) throw *this;
};
#elif have_source_location == 1
CAENFlashException::CAENFlashException(const int_least32_t& code, const std::source_location& loc) : Exception(code, errorStrings(code), loc)
{
  if(code != SUCCESS) throw *this;
};
#else
CAENFlashException::CAENFlashException(const int_least32_t& code): Exception(code, errorStrings(code))
{
  if(code != SUCCESS) throw *this;
};
#endif

}  // namespace CAEN
