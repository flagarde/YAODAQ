#include "BoardInfos.hpp"

namespace yaodaq
{

BoardInfos::BoardInfos(const std::string& roomName, const std::string& rackName, const std::string& crateName, const std::string& name, const std::string& type, const toml::value& parameters,
                       const toml::value& connectorParameters,const Category& category)
    : Infos(roomName, rackName, crateName, name, type,category)
{
  m_ConnectorParameters = connectorParameters;
  m_Parameters          = parameters;
}

BoardInfos::BoardInfos(const Infos& infos, const toml::value& parameters, const toml::value& connectorParameters): Infos(infos)
{
  m_ConnectorParameters = connectorParameters;
  m_Parameters          = parameters;
}

};
