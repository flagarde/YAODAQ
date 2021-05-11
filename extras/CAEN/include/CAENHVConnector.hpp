#pragma once

#include "Connector.hpp"

namespace CAEN
{
class CAENHVConnector: public Connector
{
public:
  CAENHVConnector();
  virtual ~CAENHVConnector() = default;
  void DoConnect() final;
  void DoDisconnect() final;
};

}  // namespace CAEN
