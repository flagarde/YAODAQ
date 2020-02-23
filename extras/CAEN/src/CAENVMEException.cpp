#include "CAENVMEException.hpp"

#include "CAENVMElib.h"
#include "CAENVMEtypes.h"

#include <cstdint>

namespace CAEN
{
const char* CAENVMEException::errorStrings(const std::int_least32_t& code)
{
  return CAENVME_DecodeError(CVErrorCodes(code));
}

#if experimental_have_source_location == 1
CAENVMEException::CAENVMEException(const int&                         code,
                           std::experimental::source_location loc)
    : Exception(code, errorStrings(code), loc)
{
  if(code != 0) throw;
};
#elif have_source_location == 1
CAENVMEException::CAENVMEException(const int& code, std::source_location loc)
    : Exception(code, errorStrings(code), loc)
{
  if(code != 0) throw;
};
#else
CAENVMEException::CAENVMEException(const int& code): Exception(code, errorStrings(code))
{
  if(code != 0) throw;
};
#endif

}  // namespace CAEN
