#include "CAENVMEException.hpp"

#include "CAENVMElib.h"
#include "CAENVMEtypes.h"

#include "magic_enum.hpp"

namespace CAEN
{
const char* CAENVMEException::errorStrings(const int& code)
{
  return CAENVME_DecodeError(static_cast<CVErrorCodes>(code));
}

#if experimental_have_source_location == 1
CAENVMEException::CAENVMEException(const int&                         code,
                           std::experimental::source_location loc)
    : Exception(code, errorStrings(code), loc)
{
  if(code != 0) throw *this;
};
#elif have_source_location == 1
CAENVMEException::CAENVMEException(const int& code, std::source_location loc)
    : Exception(code, errorStrings(code), loc)
{
  if(code != 0) throw *this;
};
#else
CAENVMEException::CAENVMEException(const int& code): Exception(code, errorStrings(code))
{
  if(code != 0) throw *this;
};
#endif

std::string CAENVMEException::toString() const
{
  return std::string(
      magic_enum::enum_name(magic_enum::enum_cast<CVErrorCodes>(getCode()).value()));
}

}  // namespace CAEN
