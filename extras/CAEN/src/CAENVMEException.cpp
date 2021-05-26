#include "CAENVMEException.hpp"

#include "CAENVMElib.h"
#include "CAENVMEtypes.h"

namespace CAEN
{
const char* CAENVMEException::errorStrings(const int_least32_t& code)
{
  return CAENVME_DecodeError(static_cast<CVErrorCodes>(code));
}

CAENVMEException::CAENVMEException(const int_least32_t& code, const source_location& location): Exception(code, errorStrings(code), location)
{
  if(code != cvSuccess) throw *this;
};

}  // namespace CAEN
