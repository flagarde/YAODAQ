#include "CAENHVException.hpp"
#include "CAENHVWrapper.h"

namespace CAEN
{
  const char* CAENHVException::errorStrings(const int_least32_t& code)
{
  switch(code)
  {
    case CAENHV_OK:
      return "Command wrapper correctly executed";
    case CAENHV_SYSERR:
      return "Error of operatived system";
    case CAENHV_WRITEERR:
      return "Write error in communication channel";
    case CAENHV_READERR:
      return "Read error in communication channel";
    case CAENHV_TIMEERR:
      return "Time out in server communication";
    case CAENHV_DOWN:
      return "Command Front End application is down";
    case CAENHV_NOTPRES:
      return "Communication with system not yet connected by a Login command";
    case CAENHV_SLOTNOTPRES:
      return "Communication with a not present board/slot";
    case CAENHV_NOSERIAL:
      return "Communication with RS232 not yet implemented";
    case CAENHV_MEMORYFAULT:
      return "User memory not sufficient";
    case CAENHV_OUTOFRANGE:
      return "Value out of range";
    case CAENHV_EXECCOMNOTIMPL:
      return "Execute command not yet implemented";
    case CAENHV_GETPROPNOTIMPL:
      return "Get Property not yet implemented";
    case CAENHV_SETPROPNOTIMPL:
      return "Set Property not yet implemented";
    case CAENHV_PROPNOTFOUND:
      return "Property not found";
    case CAENHV_EXECNOTFOUND:
      return "Execute command not found";
    case CAENHV_NOTSYSPROP:
      return "No System property";
    case CAENHV_NOTGETPROP:
      return "No get property";
    case CAENHV_NOTSETPROP:
      return "No set property";
    case CAENHV_NOTEXECOMM:
      return "No execute command";
    case CAENHV_SYSCONFCHANGE:
      return "Device configuration changed";
    case CAENHV_PARAMPROPNOTFOUND:
      return "Property of param not found";
    case CAENHV_PARAMNOTFOUND:
      return "Param not found";
    case CAENHV_NODATA:
      return "No data present";
    case CAENHV_DEVALREADYOPEN:
      return "Device already open";
    case CAENHV_TOOMANYDEVICEOPEN:
      return "To Many devices opened";
    case CAENHV_INVALIDPARAMETER:
      return "Function Parameter not valid";
    case CAENHV_FUNCTIONNOTAVAILABLE:
      return "Function not available for the connected device";
    case CAENHV_SOCKETERROR:
      return "Socket error";
    case CAENHV_COMMUNICATIONERROR:
      return "Communication error";
    case CAENHV_NOTYETIMPLEMENTED:
      return "Not yet implemented";
    case CAENHV_CONNECTED:
      return "Device already connected";
    case CAENHV_NOTCONNECTED:
      return "Device not connected";
    case CAENHV_OS:
      return "Operating system error";
    case CAENHV_LOGINFAILED:
      return "Login failed";
    case CAENHV_LOGOUTFAILED:
      return "Logout failed";
    case CAENHV_LINKNOTSUPPORTED:
      return "Link type not supported";
    case CAENHV_USERPASSFAILED:
      return "Login failed for username/password";
    default :
      return "Error code unknown";
  }
}


CAENHVException::CAENHVException(const int_least32_t& code, const SourceLocation& location) : Exception(code, errorStrings(code), location)
{
  if(code != CAENHV_OK) throw *this;
};


}  // namespace CAEN
