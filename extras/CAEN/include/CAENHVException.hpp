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
  virtual std::string toString() const final;
private:
  CAENHVException() = delete;
  virtual const char* errorStrings(const int& code) final;
  
  enum HV_ERROR
  {
    CAENHV_OK,
    CAENHV_SYSERR,
 CAENHV_WRITEERR,
 CAENHV_READERR,
 CAENHV_TIMEERR,
 CAENHV_DOWN,
 CAENHV_NOTPRES,
 CAENHV_SLOTNOTPRES,
 CAENHV_NOSERIAL,
 CAENHV_MEMORYFAULT,
 CAENHV_OUTOFRANGE,
 CAENHV_EXECCOMNOTIMPL,
 CAENHV_GETPROPNOTIMPL,
 CAENHV_SETPROPNOTIMPL,
 CAENHV_PROPNOTFOUND,
 CAENHV_EXECNOTFOUND,
 CAENHV_NOTSYSPROP,
 CAENHV_NOTGETPROP,
 CAENHV_NOTSETPROP,
 CAENHV_NOTEXECOMM,
 CAENHV_SYSCONFCHANGE,
 CAENHV_PARAMPROPNOTFOUND,
 CAENHV_PARAMNOTFOUND,
 CAENHV_NODATA,
 CAENHV_DEVALREADYOPEN,
 CAENHV_TOOMANYDEVICEOPEN,
 CAENHV_INVALIDPARAMETER,
 CAENHV_FUNCTIONNOTAVAILABLE,
 CAENHV_SOCKETERROR,
 CAENHV_COMMUNICATIONERROR,
 CAENHV_NOTYETIMPLEMENTED,
 CAENHV_CONNECTED =(0x1000 + 1),
 CAENHV_NOTCONNECTED=(0x1000 + 2),
 CAENHV_OS=(0x1000 + 3),
 CAENHV_LOGINFAILED=(0x1000 + 4),
 CAENHV_LOGOUTFAILED=(0x1000 + 5),
 CAENHV_LINKNOTSUPPORTED=(0x1000 + 6),  // Rel. 1.2
 CAENHV_USERPASSFAILED=(0x1000 + 7),  // Rel. 5.0
  };
  
};

}  // namespace CAEN
