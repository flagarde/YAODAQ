#pragma once

#include "Exception.hpp"

namespace CAEN
{
class CAENDigitizerException: public Exception
{
public:
  CAENDigitizerException(const int_least32_t& code, const SourceLocation& location = SourceLocation());

private:
  CAENDigitizerException() = delete;
  const char* errorStrings(const int_least32_t& code);
};

}  // namespace CAEN
