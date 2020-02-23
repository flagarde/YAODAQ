#ifndef CAENFLASHERROR_HPP
#define CAENFLASHERROR_HPP
#include "Error.hpp"

#include <cstdint>

namespace CAEN
{
class CAENFlashError: public Error
{
public:
#if experimental_have_source_location == 1
  CAENFlashError(const int&                         code = 0,
                 std::experimental::source_location loc =
                     std::experimental::source_location::current());
#elif have_source_location == 1
  CAENFlashError(const int&           code = 0,
                 std::source_location loc  = std::source_location::current());
#else
  CAENFlashError(const int& code = 0)
#endif
private:
  CAENFlashError() = delete;
  virtual const char* ErrorStrings(const std::int_least32_t& code) final;
};

}  // namespace CAEN
#endif
