#pragma once

#include "Exception.hpp"

namespace CAEN
{
  
class CAENVMEException: public Exception
{
public:
#if experimental_have_source_location == 1
  CAENVMEException(const int& code = 0, std::experimental::source_location loc = std::experimental::source_location::current());
#elif have_source_location == 1
  CAENVMEException(const int& code = 0, std::source_location loc  = std::source_location::current());
#else
  CAENVMEException(const int& code = 0);
#endif
  virtual std::string toString() const final;
private:
  CAENVMEException() = delete;
  virtual const char* errorStrings(const int& code) final;
  
};

}  // namespace CAEN
