#pragma once

#include "Exception.hpp"

#include <cstdint>

namespace CAEN
{
class CAENHVException: public Exception
{
public:
#if experimental_have_source_location == 1
  CAENHVException(const int&                         code = 0,
              std::experimental::source_location loc =
                  std::experimental::source_location::current());
#elif have_source_location == 1
  CAENHVException(const int&           code = 0,
              std::source_location loc  = std::source_location::current());
#else
  CAENHVException(const int& code = 0)
#endif
private:
  CAENHVException() = delete;
  virtual const char* errorStrings(const std::int_least32_t& code) final;
};

}  // namespace CAEN
