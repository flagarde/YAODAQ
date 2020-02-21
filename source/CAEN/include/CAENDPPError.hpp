#ifndef CAENDPPERROR_HPP
#define CAENDPPERROR_HPP
#include "Error.hpp"

#include <cstdint>

namespace CAEN
{
class CAENDPPError: public Error
{
public:
#if experimental_have_source_location == 1
  CAENDPPError(const int&                         code = 0,
               std::experimental::source_location loc =
                   std::experimental::source_location::current());
#elif have_source_location == 1
  CAENDPPError(const int&           code = 0,
               std::source_location loc  = std::source_location::current());
#else
  CAENDPPError(const int& code = 0)
#endif
private:
  CAENDPPError() = delete;
  virtual const char* ErrorStrings(const std::int_least32_t& code) final;
};

}  // namespace CAEN
#endif
