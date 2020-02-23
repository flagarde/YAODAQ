#ifndef CAENVMEERROR_HPP
#define CAENVMEERROR_HPP
#include "Error.hpp"

#include <cstdint>

namespace CAEN
{
class CAENVMEError: public Error
{
public:
#if experimental_have_source_location == 1
  CAENVMEError(const int&                         code = 0,
               std::experimental::source_location loc =
                   std::experimental::source_location::current());
#elif have_source_location == 1
  CAENVMEError(const int&           code = 0,
               std::source_location loc  = std::source_location::current());
#else
  CAENVMEError(const int& code = 0)
#endif
private:
  CAENVMEError() = delete;
  virtual const char* ErrorStrings(const std::int_least32_t& code) final;
};

}  // namespace CAEN
#endif
