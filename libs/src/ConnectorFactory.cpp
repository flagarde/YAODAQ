#include "ConnectorFactory.hpp"

#include "Exception.hpp"
#include "Internal.hpp"
#include "StatusCode.hpp"
#include "spdlog.h"
#include "toml.hpp"

ConnectorFactory::ConnectorFactory()
{
  checkEnvironmentVariable();
  m_Loader.FindPluginsAtDirectory(m_Path);
  for(auto connector: m_Loader.BuildAndResolvePlugin<Connector>())
  {
    m_Plugins.emplace(connector->getType(), connector);
    m_StringConnectorNames += (connector->getType() + " ");
  }
}

void ConnectorFactory::checkEnvironmentVariable()
{
  if(std::getenv("YAODAQ_CONNECTOR_DIR") != nullptr) m_Path = std::string(std::getenv("YAODAQ_CONNECTOR_DIR"));
  if(m_Path != "") { spdlog::info("Loading libraries in folder {}", m_Path); }
  else
    throw Exception(StatusCode::NOT_FOUND, "YAODAQ_CONNECTOR_DIR environmental variable not found! Can't load libraries for connectors!");
}

std::shared_ptr<Connector> ConnectorFactory::createConnector(const ConnectorInfos& infos)
{
  std::string m_Type{""};
  try
  {
    m_Type = toml::find<std::string>(infos.getParameters(), "Type");
  }
  catch(const std::out_of_range& e)
  {
    throw Exception(StatusCode::NOT_FOUND, "Type key not set in Connector !");
  }
  if(m_Plugins.find(m_Type) != m_Plugins.end())
  {
    spdlog::info("Creating Connector {}", m_Type);
    m_Connectors.emplace(infos.getID(), m_Plugins[m_Type]);
    m_Connectors[infos.getID()]->setInfos(infos);
    return m_Connectors[infos.getID()];
  }
  throw Exception(StatusCode::NOT_FOUND, "Connector " + m_Type + " not loaded ! Connector loaded are : " + m_StringConnectorNames);
  return std::shared_ptr<Connector>{nullptr};
}
