#pragma once

#include <string>
#include "ConnectorInfos.hpp"

class Connector
{
public:
  Connector(const std::string& type,const ConnectorInfos& infos);
  virtual std::int32_t Connect()=0;
  virtual void Disconnect()=0;
  virtual ~Connector(){};
  void printParameters();
  toml::value getParameters();
  virtual void verifyParameters(){};
protected:
  std::string m_Type{""};
  std::int32_t m_Handle{0};
  ConnectorInfos m_Infos;
};
