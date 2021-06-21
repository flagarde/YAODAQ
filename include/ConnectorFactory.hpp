#pragma once

#include "Connector.hpp"
#include "ConnectorInfos.hpp"
#include "PluginLoader.hpp"

#include <memory>
#include <string>
#include <unordered_map>

namespace yaodaq
{

class ConnectorFactory
{
public:
  std::shared_ptr<Connector> createConnector(const ConnectorInfos& infos);
  void                       loadConnectors();

private:
  std::string                                                 m_StringConnectorNames{""};
  std::unordered_map<int, std::shared_ptr<Connector>>         m_Connectors;
  FlakedTuna::PluginLoader                                    m_Loader;
  void                                                        checkEnvironmentVariable();
  std::string                                                 m_Path{""};
  std::unordered_map<std::string, std::shared_ptr<Connector>> m_Plugins;
  bool                                                        m_Loaded{false};
};

}
