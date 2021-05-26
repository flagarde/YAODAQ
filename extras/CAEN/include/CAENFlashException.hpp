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
  CAENFlashException(const int_least32_t& code, const source_location& location = source_location::current());

private:
  CAENFlashException() = delete;
  const char* errorStrings(const int_least32_t& code);
};

}  // namespace CAEN
