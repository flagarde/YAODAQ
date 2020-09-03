#include "ConnectorFactory.hpp"

#include "Exception.hpp"
#include "Internal.hpp"
#include "StatusCode.hpp"
#include "toml.hpp"

void ConnectorFactory::loadConnectors()
{
  if(m_Loaded == false)
  {
    checkEnvironmentVariable();
    m_Loader.FindPluginsAtDirectory(m_Path);
    for(auto connector: m_Loader.BuildAndResolvePlugin<Connector>())
    {
      m_Plugins.emplace(connector->getType(), connector);
      m_StringConnectorNames += (connector->getType() + " ");
    }
    m_Loaded = true;
  }
}

void ConnectorFactory::checkEnvironmentVariable()
{
  if(std::getenv("YAODAQ_CONNECTOR_DIR") != nullptr) m_Path = std::string(std::getenv("YAODAQ_CONNECTOR_DIR"));
  else throw Exception(StatusCode::NOT_FOUND, "YAODAQ_CONNECTOR_DIR environmental variable not found! Can't load libraries for connectors!");
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
    Exception(StatusCode::NOT_FOUND, "Type key not set in Connector !");
  }
  if(m_Plugins.find(m_Type) != m_Plugins.end())
  {
    m_Connectors.emplace(infos.getID(), m_Plugins[m_Type]);
    m_Connectors[infos.getID()]->setInfos(infos);
    return m_Connectors[infos.getID()];
  }
  else
    throw Exception(StatusCode::NOT_FOUND, "Connector " + m_Type + " not loaded ! Connector loaded are : " + m_StringConnectorNames);
}
