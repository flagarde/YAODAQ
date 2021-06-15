#pragma once

#include "ConnectorInfos.hpp"
#include "Internal.hpp"
#include "Message.hpp"

#include <string>

namespace yaodaq
{

class Connector
{
public:
  Connector(const std::string& type);
  virtual ~Connector() = default;
  void             printParameters();
  toml::value      getParameters();
  virtual void     verifyParameters();
  int32_t          Connect();
  void             Disconnect();
  std::string      getType();
  void             setInfos(const ConnectorInfos& infos);
  bool             isConnected();
  virtual Response sendCommand(const Command&) const { return Response(); };

protected:
  std::string  m_Type;
  virtual void DoConnect()    = 0;
  virtual void DoDisconnect() = 0;
  Connector()                 = default;
  int32_t        m_Handle{0};
  ConnectorInfos m_Infos;
};

};
