#pragma once

#include "Connector.hpp"

#include <string>

struct snmp_session;

namespace SNMP
{

class SNMPConnector: public yaodaq::Connector
{
public:
  SNMPConnector();
  virtual ~SNMPConnector() = default;
  void DoDisconnect() final;
  void DoConnect() final;
  yaodaq::Response     sendCommand(const yaodaq::Command& command) const final;

private:
  yaodaq::Response send(const yaodaq::Command&) const;
  yaodaq::Response receive(const yaodaq::Command&) const;
  char findType(const yaodaq::Response&) const;
  void    verifyParameters() final;
  ::snmp_session* m_session{nullptr};
  std::string     m_MIBFilename;
  std::string     m_MIBPath;
  std::string     m_IP;
  std::string     m_readCommunity{"public"};
  long            m_timeout{300000};  // timeout (us)
  int             m_retries{2};       // retries
  int             m_version{1};
};

};  // namespace SNMP
