#include "CAENFlashException.hpp"

#include <cstdint>

namespace CAEN
{
const char* CAENFlashException::errorStrings(const std::int_least32_t& code)
{
  switch(code)
  {
    case 0:
      return "Success";
    case -1:
      return "Access Failed";
    case -2:
      return "Parameter not allowed";
    case -3:
      return "File open error";
    case -4:
      return "Malloc error";
    case -5:
      return "Uninitialized";
    case -6:
      return "Not implemented";
    case -7:
      return "Unsupported flash device";
  }
  return "Unknown error";
}

#if experimental_have_source_location == 1
CAENFlashException::CAENFlashException(const int&                         code,
                               std::experimental::source_location loc)
    : Exception(code, errorStrings(code), loc)
{
  if(code != 0) throw;
};
#elif have_source_location == 1
CAENFlashException::CAENFlashException(const int& code, std::source_location loc)
    : Exception(code, errorStrings(code), loc)
{
  if(code != 0) throw;
};
#else
CAENFlashException::CAENFlashException(const int& code): Exception(code, errorStrings(code))
{
  if(code != 0) throw;
};
#endif

}  // namespace CAEN
