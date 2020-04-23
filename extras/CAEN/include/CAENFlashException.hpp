#pragma once

#include "Exception.hpp"

namespace CAEN
{
class CAENFlashException: public Exception
{
public:
#if experimental_have_source_location == 1
  CAENFlashException(const int&                         code = 0,
                 std::experimental::source_location loc =
                     std::experimental::source_location::current());
#elif have_source_location == 1
  CAENFlashException(const int&           code = 0,
                 std::source_location loc  = std::source_location::current());
#else
  CAENFlashException(const int& code = 0)
#endif
  virtual std::string toString() const final;
private:
  CAENFlashException() = delete;
  virtual const char* errorStrings(const int& code) final;
  
  enum FLASH_API_ERROR_CODES
  {
  FLASH_API_SUCCESS                  = 0,
  FLASH_API_ACCESS_FAILED            = -1,
  FLASH_API_PARAMETER_NOT_ALLOWED    = -2,
  FLASH_API_FILE_OPEN_ERROR          = -3,
  FLASH_API_MALLOC_ERROR             = -4,
  FLASH_API_UNINITIALIZED            = -5,
  FLASH_API_NOT_IMPLEMENTED          = -6,
  FLASH_API_UNSUPPORTED_FLASH_DEVICE = -7
  };
};
}  // namespace CAEN
