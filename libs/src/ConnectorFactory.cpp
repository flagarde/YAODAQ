#include "ConnectorFactory.hpp"

#include "Exception.hpp"
#include "Internal.hpp"
#include "spdlog.h"
#include "toml.hpp"

#include <iostream>

ConnectorFactory::ConnectorFactory()
{
  checkEnvironmentVariable();
  m_Loader.FindPluginsAtDirectory(m_Path);
}

void ConnectorFactory::checkEnvironmentVariable()
{
  if(m_Path = std::getenv("YAODAQ_CONNECTOR_DIR"); m_Path != "") { spdlog::info("Loading libraries in folder {}", m_Path); }
  else
  {
    spdlog::error("YAODAQ_CONNECTOR_DIR environmental variable not found! Can't load libraries for connectors!");
  }
}

std::shared_ptr<Connector> ConnectorFactory::createConnector(const ConnectorInfos& infos)
{
  std::vector<std::shared_ptr<Connector>> plugins = m_Loader.BuildAndResolvePlugin<Connector>();
  // First I need to see if a type is given to create it
  std::string m_Type{""};
  try
  {
    m_Type = toml::find<std::string>(infos.getParameters(), "Type");
  }
  catch(const std::out_of_range& e)
  {
    throw Exception(STATUS_CODE_NOT_FOUND, "Type key not set in Connector !");
  }
  for(unsigned int i = 0; i != plugins.size(); ++i)
  {
    if(plugins[i]->getType() == m_Type)
    {
      spdlog::info("Creating Connector {}", plugins[i]->getType());
      m_Connectors.emplace(infos.getID(), plugins[i]);
      m_Connectors[infos.getID()]->setInfos(infos);
      return m_Connectors[infos.getID()];
    }
  }
  std::string all{""};
  for(unsigned int i = 0; i != plugins.size(); ++i) { all = all + plugins[i]->getType() + " "; }
  throw Exception(STATUS_CODE_NOT_FOUND, "Connector " + m_Type + " not loaded \n Connector loaded are : " + all);
  return std::shared_ptr<Connector>{nullptr};
}
