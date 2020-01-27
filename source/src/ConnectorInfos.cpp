#include "ConnectorInfos.hpp"
#include <iostream>
#include "serializer.hpp"

ConnectorInfos::ConnectorInfos(const toml::value& params,const bool& isShared,const int& id):m_Parameters(params),m_IsSharedConnector(isShared),m_ID(id)
{
  
}

int ConnectorInfos::getID()
{
  return m_ID;
}

int ConnectorInfos::getID() const
{
  return m_ID;
}

bool ConnectorInfos::isSharedConnector()
{
  return m_IsSharedConnector;
}
  
bool ConnectorInfos::isSharedConnector() const 
{
  return m_IsSharedConnector;
}

toml::value& ConnectorInfos::getParameters()
{
  return m_Parameters;
}

toml::value ConnectorInfos::getParameters() const
{
  return m_Parameters;
}

void ConnectorInfos::printParameters()
{
  std::cout<<m_Parameters<<std::endl;
}

void ConnectorInfos::printParameters() const
{
  std::cout<<m_Parameters<<std::endl;
}
