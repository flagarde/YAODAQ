#pragma once

#include "Exception.hpp"

namespace CAEN
{
class CAENCommException: public yaodaq::Exception
{
public:
  CAENCommException(const int_least32_t& code, const source_location& location = source_location::current());

private:
  CAENCommException() = delete;
  const char* errorStrings(const int_least32_t& code);
};

}  // namespace CAEN
