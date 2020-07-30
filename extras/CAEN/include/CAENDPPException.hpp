#pragma once

#include "Exception.hpp"

namespace CAEN
{
class CAENDPPException: public Exception
{
public:
  CAENDPPException(const int_least32_t& code, const SourceLocation& location = SourceLocation());

private:
  CAENDPPException() = delete;
  const char* errorStrings(const int_least32_t& code);
};

}  // namespace CAEN
