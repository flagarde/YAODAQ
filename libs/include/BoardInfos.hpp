#pragma once

#include "Infos.hpp"
#include "toml.hpp"

class BoardInfos: public Infos
{
public:
  BoardInfos(const std::string& roomName = "", const std::string& rackName = "", const std::string& crateName = "", const std::string& name = "",
             const std::string& type = "", const toml::value& parameters = {}, const toml::value& connectorParameters = {});
  void print(std::ostream& os = std::cout, const std::string shift = "");
  BoardInfos(const Infos& infos, const toml::value& parameters = {}, const toml::value& connectorParameters = {});

private:
  toml::value m_ConnectorParameters;
  toml::value m_Parameters;
};
