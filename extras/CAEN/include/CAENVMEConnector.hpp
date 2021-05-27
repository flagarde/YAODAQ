#pragma once
#include "Connector.hpp"

#include <string>
#include <unordered_map>

namespace CAEN
{
class CAENVMEConnector: public yaodaq::Connector
{
public:
  CAENVMEConnector();
  void DoConnect() final;
  void DoDisconnect() final;
  virtual ~CAENVMEConnector() = default;

private:
  void                                verifyParameters() final;
  std::string                                 m_Model{""};
  short                                       m_ConetNode{0};
  short                                       m_LinkNumber{0};
  static std::unordered_map<std::string, int> m_ModelList;
};

}  // namespace CAEN
