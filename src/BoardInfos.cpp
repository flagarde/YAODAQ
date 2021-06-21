#include "BoardInfos.hpp"

namespace yaodaq
{

BoardInfos::BoardInfos(const std::string& roomName, const std::string& rackName, const std::string& crateName,const int& slot, const std::string& name, const std::string& type, const toml::value& parameters,
                       const toml::value& connectorParameters,const CLASS& _class)
: Infos(roomName, rackName, crateName,slot,_class,type,name)
{
  m_ConnectorParameters = connectorParameters;
  m_Parameters          = parameters;
}

BoardInfos::BoardInfos(const Infos& infos, const toml::value& parameters, const toml::value& connectorParameters): Infos(infos)
{
  m_ConnectorParameters = connectorParameters;
  m_Parameters          = parameters;
}

}
