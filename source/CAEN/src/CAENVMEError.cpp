#include "CAENVMEError.hpp"
#include "CAENVMEtypes.h"
#include "CAENVMElib.h"
#include <cstdint>

namespace CAEN
{
    
const char* CAENVMEError::ErrorStrings(const std::int_least32_t& code)
{
    return CAENVME_DecodeError(CVErrorCodes(code));
}

#if experimental_have_source_location == 1
CAENVMEError::CAENVMEError(const int& code,std::experimental::source_location loc):Error(code,ErrorStrings(code),loc)
{
  if(code!=0) throw *this;
};
#elif have_source_location == 1
CAENVMEError::CAENVMEError(const int& code,std::source_location loc):Error(code,ErrorStrings(code),loc)
{
  if(code!=0) throw *this;
};
#else
CAENVMEError::CAENVMEError(const int& code):Error(code,ErrorStrings(code))
{
  if(code!=0) throw *this;
};
#endif

}
