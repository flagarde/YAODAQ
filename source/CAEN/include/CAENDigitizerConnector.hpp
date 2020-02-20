#pragma once

#include <string>
#include <unordered_map>

#include "Connector.hpp"

namespace CAEN 
{

class CAENDigitizerConnector : public Connector
{
public:
  CAENDigitizerConnector(const ConnectorInfos& infos);
  virtual void Disconnect() final;
  virtual std::int32_t Connect() final;
private:
  virtual void verifyParameters() final;
  std::string m_ConnectionType{""};
  std::string m_Model{""};
  int m_LinkNum{0};
  int m_ConetNode{0};
  std::uint32_t m_VMEBaseAddress{0};
  static std::unordered_map<std::string,int> m_ModelList;
  static std::unordered_map<std::string,int> m_ConnectionTypeList;
};

}
