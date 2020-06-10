#pragma once

#include "Exception.hpp"

namespace CAEN
{

enum FLASH_API_ERROR_CODES : int_least32_t
{
  SUCCESS                  = 0,
  ACCESS_FAILED            = -1,
  PARAMETER_NOT_ALLOWED    = -2,
  FILE_OPEN_ERROR          = -3,
  MALLOC_ERROR             = -4,
  UNINITIALIZED            = -5,
  NOT_IMPLEMENTED          = -6,
  UNSUPPORTED_FLASH_DEVICE = -7
};

class CAENFlashException: public Exception
{
  
public:
#if experimental_have_source_location == 1
  CAENFlashException(const int_least32_t& code, const std::experimental::source_location& loc = std::experimental::source_location::current());
#elif have_source_location == 1
  CAENFlashException(const int_least32_t& code, const std::source_location& loc  = std::source_location::current());
#else
  CAENFlashException(const int_least32_t& code);
#endif
  
private:
  CAENFlashException() = delete;
  const char* errorStrings(const int_least32_t& code);
  
};

}  // namespace CAEN
