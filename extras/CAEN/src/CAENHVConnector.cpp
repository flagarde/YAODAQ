#include "CAENHVConnector.hpp"

namespace CAEN
{
  CAENHVConnector::CAENHVConnector():Connector("CAENHV"){}
  void CAENHVConnector::DoConnect(){};
  void CAENHVConnector::DoDisconnect(){};
  
  Response CAENHVConnector::sendCommand(const Command&)
  {
    return Response();
  }
  
}
