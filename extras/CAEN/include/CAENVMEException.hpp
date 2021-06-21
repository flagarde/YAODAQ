#pragma once

#include "Exception.hpp"

namespace CAEN
{
class CAENVMEException: public yaodaq::Exception
{
public:
  CAENVMEException(const int_least32_t& code, const source_location& location = source_location::current());

private:
  CAENVMEException() = delete;
  const char* errorStrings(const int_least32_t& code);
};

}  // namespace CAEN
