#pragma once

#include "ConnectorInfos.hpp"
#include "PluginRegistry.hpp"

#include <string>

class Connector
{
public:
  Connector(const std::string& type, const ConnectorInfos& infos);
  virtual ~Connector(){};
  void         printParameters();
  toml::value  getParameters();
  virtual void verifyParameters();
  int32_t      Connect();
  void         Disconnect();
  std::string  getType();
  void         setInfos(const ConnectorInfos& infos);

protected:
  virtual void DoConnect()    = 0;
  virtual void DoDisconnect() = 0;
  Connector(){};
  std::string    m_Type{""};
  int32_t        m_Handle{0};
  ConnectorInfos m_Infos;
};
