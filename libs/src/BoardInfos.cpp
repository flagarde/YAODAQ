#include "BoardInfos.hpp"

BoardInfos::BoardInfos(const std::string& roomName, const std::string& rackName,
                       const std::string& crateName, const std::string& name,
                       const std::string& type, const toml::value& parameters,
                       const toml::value& connectorParameters)
    : Infos(roomName, rackName, crateName, name, type)
{
  m_ConnectorParameters = connectorParameters;
  m_Parameters          = parameters;
}

void BoardInfos::print(std::ostream& os, const std::string shift)
{
  Infos::print(os, shift);
  os << shift << "Parameters :" << std::endl;
  os << shift << m_Parameters << std::endl;
  os << shift << "Connector Parameters :" << std::endl;
  os << shift << m_ConnectorParameters << std::endl;
}
