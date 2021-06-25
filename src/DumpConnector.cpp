#include "DumpConnector.hpp"

namespace yaodaq
{

DumpConnector::DumpConnector() : Connector("DumpConnector")
{

}

void DumpConnector::DoConnect()
{

}

void DumpConnector::DoDisconnect()
{

}

Response DumpConnector::sendCommand(const Command& command) const
{
  return Response();
}

}
