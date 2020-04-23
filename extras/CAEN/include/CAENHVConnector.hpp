#pragma once

#include "Connector.hpp"

namespace CAEN
{
class CAENHVConnector: public Connector
{
public:
  CAENHVConnector(const ConnectorInfos& infos={});
  virtual void DoConnect() final;
  virtual void DoDisconnect() final;
};

}  // namespace CAEN

