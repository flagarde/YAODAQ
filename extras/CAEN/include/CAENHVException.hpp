#pragma once

#include "Exception.hpp"

namespace CAEN
{
class CAENHVException: public Exception
{
public:
  CAENHVException(const int_least32_t& code, const source_location& location = source_location::current());

private:
  CAENHVException() = delete;
  const char* errorStrings(const int_least32_t& code);
};

}  // namespace CAEN
