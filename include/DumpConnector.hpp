#pragma once
#include "Connector.hpp"

namespace yaodaq
{

class DumpConnector : public Connector
{
public:
  DumpConnector();
  void DoConnect() final;
  void DoDisconnect() final;
  Response sendCommand(const Command&) const final;
};

}
