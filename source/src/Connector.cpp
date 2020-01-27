#include "Connector.hpp"

Connector::Connector(const std::string& type,const ConnectorInfos& infos):m_Type(type),m_Infos(infos)
{
  
}

toml::value Connector::getParameters()
{
  return m_Infos.getParameters();
}


void Connector::printParameters()
{
  m_Infos.printParameters();
}
