#pragma once

#include "Exception.hpp"

namespace CAEN
{
class CAENVMEException: public Exception
{
public:
  CAENVMEException(const int_least32_t& code, const SourceLocation& location = SourceLocation());

private:
  CAENVMEException() = delete;
  const char* errorStrings(const int_least32_t& code);
};

}  // namespace CAEN
