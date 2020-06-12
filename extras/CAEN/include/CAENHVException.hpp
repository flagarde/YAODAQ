#pragma once

#include "Exception.hpp"

namespace CAEN
{
  
class CAENHVException: public Exception
{
  
public:
  CAENHVException(const int_least32_t& code, const SourceLocation& location = SourceLocation());

private:
  CAENHVException() = delete;
  const char* errorStrings(const int_least32_t& code);
  
};

}  // namespace CAEN
