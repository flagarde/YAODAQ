#pragma once

#include "Connector.hpp"

#include <string>

struct snmp_session;

namespace SNMP
{
  
  class SNMPConnector: public Connector
  {
  public:
    SNMPConnector();
    virtual ~SNMPConnector()=default;
    virtual void         DoDisconnect() final;
    virtual void DoConnect() final;
    Response sendCommand(const Command& command) const;
  private:
    virtual void                                verifyParameters() final;
    ::snmp_session* m_session{nullptr};
    std::string m_MIBFilename{""};
    std::string m_MIBPath{""};
    std::string m_IP{""};
    std::string m_readCommunity{"public"};
    long m_timeout{300000};   // timeout (us)
    int  m_retries{2};        // retries
    int  m_version{1};
  };
  
}  // namespace SNMP
