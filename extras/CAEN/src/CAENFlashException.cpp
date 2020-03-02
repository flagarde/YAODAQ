#include "CAENFlashException.hpp"

namespace CAEN
{
  
const char* CAENFlashException::errorStrings(const int& code)
{
  switch(code)
  {
    case FLASH_API_SUCCESS:
      return "Success";
    case FLASH_API_ACCESS_FAILED:
      return "Access Failed";
    case FLASH_API_PARAMETER_NOT_ALLOWED:
      return "Parameter not allowed";
    case FLASH_API_FILE_OPEN_ERROR:
      return "File open error";
    case FLASH_API_MALLOC_ERROR:
      return "Malloc error";
    case FLASH_API_UNINITIALIZED:
      return "Uninitialized";
    case FLASH_API_NOT_IMPLEMENTED:
      return "Not implemented";
    case FLASH_API_UNSUPPORTED_FLASH_DEVICE:
      return "Unsupported flash device";
  }
  throw Exception(STATUS_CODE_INVALID_PARAMETER);
}

std::string CAENFlashException::toString() const
{
  return std::string(magic_enum::enum_name(magic_enum::enum_cast<FLASH_API_ERROR_CODES>(getCode()).value()));
}

#if experimental_have_source_location == 1
CAENFlashException::CAENFlashException(const int& code,std::experimental::source_location loc) : Exception(code, errorStrings(code), loc)
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
