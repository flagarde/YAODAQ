#include "CAENFlashException.hpp"

#include "magic_enum.hpp"

namespace CAEN
{
  
const char* CAENFlashException::errorStrings(const int& code)
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

std::string CAENFlashException::toString() const
{
  return std::string(magic_enum::enum_name(magic_enum::enum_cast<FLASH_API_ERROR_CODES>(getCode()).value()));
}

#if experimental_have_source_location == 1
CAENFlashException::CAENFlashException(const int& code, std::experimental::source_location loc) : Exception(code, errorStrings(code), loc)
{
  if(code != 0) throw *this;
};
#elif have_source_location == 1
CAENFlashException::CAENFlashException(const int& code, std::source_location loc) : Exception(code, errorStrings(code), loc)
{
  if(code != 0) throw *this;
};
#else
CAENFlashException::CAENFlashException(const int& code): Exception(code, errorStrings(code))
{
  if(code != 0) throw *this;
};
#endif

}  // namespace CAEN
