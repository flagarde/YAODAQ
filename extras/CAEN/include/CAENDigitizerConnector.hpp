#pragma once

#include "Connector.hpp"

#include <string>

namespace CAEN
{
class CAENDigitizerConnector: public Connector
{
public:
  CAENDigitizerConnector();
  virtual ~CAENDigitizerConnector() = default;
  void DoDisconnect() final;
  void DoConnect() final;

private:
  void verifyParameters() final;
  std::string  m_ConnectionType{""};
  std::string  m_Model{""};
  int          m_LinkNum{0};
  int          m_ConetNode{0};
  uint32_t     m_VMEBaseAddress{0};
};

}  // namespace CAEN
