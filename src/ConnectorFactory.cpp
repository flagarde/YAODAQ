#include "ConnectorFactory.hpp"

#include "Exception.hpp"
#include "StatusCode.hpp"
#include "toml.hpp"
#include "GeneralParameters.hpp"

namespace yaodaq
{

void ConnectorFactory::loadConnectors()
{
  if(!m_Loaded)
  {
    m_Loader.FindPluginsAtDirectory(GeneralParameters::getPluginsPath());
    for(const auto& connector: m_Loader.BuildAndResolvePlugin<Connector>())
    {
      m_Plugins.emplace(connector->getType(), connector);
      m_StringConnectorNames += (connector->getType() + " ");
    }
    m_Loaded = true;
  }
}

std::shared_ptr<Connector> ConnectorFactory::createConnector(const ConnectorInfos& infos)
{
  std::string type;
  try
  {
    type = toml::find<std::string>(infos.getParameters(), "Type");
  }
  catch(const std::out_of_range& e)
  {
    throw Exception(StatusCode::NOT_FOUND, "Type key not set in Connector !");
  }
  if(m_Plugins.find(type) != m_Plugins.end())
  {
    m_Connectors.emplace(infos.getID(), m_Plugins[type]);
    m_Connectors[infos.getID()]->setInfos(infos);
    return m_Connectors[infos.getID()];
  }
  else
    throw Exception(StatusCode::NOT_FOUND, "Connector {0} not loaded ! Connector loaded are : {1}", type, m_StringConnectorNames);
}

}
