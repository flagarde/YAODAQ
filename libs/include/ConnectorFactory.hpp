#pragma once

#include "Connector.hpp"
#include "ConnectorInfos.hpp"

#include <map>
#include <memory>
#include <string>
class ConnectorFactory
{
public:
  ConnectorFactory() = default;
  std::shared_ptr<Connector> createConnector(const ConnectorInfos& infos);

private:
  std::map<int, std::shared_ptr<Connector>> m_SharedConnectors;
};
