#include "Connector.hpp"

Connector::Connector(const std::string& type,const std::string& name):m_Type(type),m_Name(name)
{
  
}

void Connector::setConfiguration(const toml::value& config)
{
  m_Configs=config;
  verifyConfig();
}

void Connector::verifyConfig()
{
  
}
