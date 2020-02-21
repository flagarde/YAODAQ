#ifndef CAENVMECONNECTOR_HPP
#define CAENVMECONNECTOR_HPP
#include "Connector.hpp"

#include <string>
#include <unordered_map>

namespace CAEN {
class CAENVMEConnector : public Connector {
public:
  CAENVMEConnector(const ConnectorInfos &infos);
  virtual int Connect() final;
  virtual void Disconnect() final;

private:
  virtual void verifyParameters() final;
  std::string m_Model{""};
  short m_ConetNode{0};
  short m_LinkNumber{0};
  static std::unordered_map<std::string, int> m_ModelList;
};

} // namespace CAEN
#endif
