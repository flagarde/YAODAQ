#pragma once

#include "Exception.hpp"

namespace CAEN
{
  
class CAENCommException: public Exception
{
  
public:
  CAENCommException(const int_least32_t& code, const SourceLocation& location = SourceLocation());

private:
  CAENCommException() = delete;
  const char* errorStrings(const int_least32_t& code);
  
};

}  // namespace CAEN
