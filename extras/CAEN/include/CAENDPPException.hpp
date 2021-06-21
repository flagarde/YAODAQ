#pragma once

#include "Exception.hpp"

namespace CAEN
{
class CAENDPPException: public yaodaq::Exception
{
public:
  CAENDPPException(const int_least32_t& code, const source_location& location = source_location::current());

private:
  CAENDPPException() = delete;
  const char* errorStrings(const int_least32_t& code);
};

}  // namespace CAEN
