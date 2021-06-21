#pragma once

#include "Exception.hpp"

namespace CAEN
{
class CAENDigitizerException: public yaodaq::Exception
{
public:
  CAENDigitizerException(const int_least32_t& code, const source_location& location = source_location::current());

private:
  CAENDigitizerException() = delete;
  const char* errorStrings(const int_least32_t& code);
};

}  // namespace CAEN
