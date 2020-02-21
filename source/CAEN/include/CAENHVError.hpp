#ifndef CAENHVERROR_HPP
#define CAENHVERROR_HPP
#include "Error.hpp"

#include <cstdint>

namespace CAEN {
class CAENHVError : public Error {
public:
#if experimental_have_source_location == 1
  CAENHVError(const int &code = 0,
              std::experimental::source_location loc =
                  std::experimental::source_location::current());
#elif have_source_location == 1
  CAENHVError(const int &code = 0,
              std::source_location loc = std::source_location::current());
#else
  CAENHVError(const int& code = 0)
#endif
private:
  CAENHVError() = delete;
  virtual const char *ErrorStrings(const std::int_least32_t &code) final;
};

} // namespace CAEN
#endif
