#pragma once
#include "Connector.hpp"

#include <string>
#include <unordered_map>

namespace CAEN
{
  
class CAENVMEConnector: public Connector
{
public:
  CAENVMEConnector();
  virtual void  DoConnect() final;
  virtual void DoDisconnect() final;
  virtual ~CAENVMEConnector()=default;

private:
  virtual void                                verifyParameters() final;
  std::string                                 m_Model{""};
  short                                       m_ConetNode{0};
  short                                       m_LinkNumber{0};
  static std::unordered_map<std::string, int> m_ModelList;
};

}  // namespace CAEN
