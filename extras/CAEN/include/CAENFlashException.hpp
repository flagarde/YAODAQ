#pragma once

#include "Exception.hpp"

namespace CAEN
{
  
enum FLASH_API_ERROR_CODES : int
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
  CAENFlashException(const int& code = 0, std::experimental::source_location loc = std::experimental::source_location::current());
#elif have_source_location == 1
  CAENFlashException(const int& code = 0, std::source_location loc  = std::source_location::current());
#else
  CAENFlashException(const int& code = 0);
#endif
  virtual std::string toString() const final;
private:
  CAENFlashException() = delete;
  virtual const char* errorStrings(const int& code) final;
};

}  // namespace CAEN
