#include "CAENVMEException.hpp"

#include "CAENVMElib.h"
#include "CAENVMEtypes.h"

namespace CAEN
{
  
const char* CAENVMEException::errorStrings(const int_least32_t& code)
{
  return CAENVME_DecodeError(static_cast<CVErrorCodes>(code));
}

#if experimental_have_source_location == 1
CAENVMEException::CAENVMEException(const int_least32_t& code, const std::experimental::source_location& loc) : Exception(code, errorStrings(code), loc)
{
  if(code != cvSuccess) throw *this;
};
#elif have_source_location == 1
CAENVMEException::CAENVMEException(const int_least32_t& code, const std::source_location& loc) : Exception(code, errorStrings(code), loc)
{
  if(code != cvSuccess) throw *this;
};
#else
CAENVMEException::CAENVMEException(const int_least32_t& code): Exception(code, errorStrings(code))
{
  if(code != cvSuccess) throw *this;
};
#endif

}  // namespace CAEN
