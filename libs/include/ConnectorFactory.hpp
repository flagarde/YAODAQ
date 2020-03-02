#pragma once

#include "Connector.hpp"
#include "ConnectorInfos.hpp"
#include "PluginLoader.hpp"

#include <map>
#include <memory>
#include <string>

class ConnectorFactory
{
public:
  ConnectorFactory();
  std::shared_ptr<Connector> createConnector(const ConnectorInfos& infos);

private:
  std::map<int, std::shared_ptr<Connector>> m_Connectors;
  FlakedTuna::PluginLoader                  m_Loader;
  void                                      checkEnvironmentVariable();
  std::string                               m_Path{""};
};
