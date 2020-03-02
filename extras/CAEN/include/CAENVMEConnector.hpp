#pragma once
#include "Connector.hpp"

#include <string>

namespace CAEN
{
class CAENVMEConnector: public Connector
{
public:
  CAENVMEConnector(const ConnectorInfos& infos={});
  virtual void  DoConnect() final;
  virtual void DoDisconnect() final;

private:
  virtual void                                verifyParameters() final;
  std::string                                 m_Model{""};
  short                                       m_ConetNode{0};
  short                                       m_LinkNumber{0};
};

}  // namespace CAEN
