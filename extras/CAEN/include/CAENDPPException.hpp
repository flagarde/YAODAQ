#pragma once

#include "Exception.hpp"

namespace CAEN
{
  
class CAENDPPException: public Exception
{
  
public:
#if experimental_have_source_location == 1
  CAENDPPException(const int_least32_t& code, const std::experimental::source_location& loc = std::experimental::source_location::current());
#elif have_source_location == 1
  CAENDPPException(const int_least32_t& code, const std::source_location& loc  = std::source_location::current());
#else
  CAENDPPException(const int_least32_t& code);
#endif
  
private:
  CAENDPPException() = delete;
  const char* errorStrings(const int_least32_t& code);
  
};

}  // namespace CAEN
