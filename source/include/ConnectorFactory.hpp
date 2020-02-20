#pragma once

#include "Connector.hpp"
#include "ConnectorInfos.hpp"
#include <memory>
#include <map>
#include <string>
class ConnectorFactory
{
public:
  ConnectorFactory(){};
  std::shared_ptr<Connector> createConnector(const ConnectorInfos& infos);
private:
  std::map<int,std::shared_ptr<Connector>> m_SharedConnectors;
};
