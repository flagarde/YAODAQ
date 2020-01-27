#ifndef CAENVMECONNECTOR_HPP
#define CAENVMECONNECTOR_HPP
#include <string>
#include <unordered_map>

#include "Connector.hpp"

namespace CAEN
{

class CAENVMEConnector : public Connector
{ 
public:
  CAENVMEConnector(const ConnectorInfos& infos);
  virtual int Connect() final;
  virtual void Disconnect() final;
private:
  virtual void verifyParameters() final;
  std::string m_Model{""};
  short m_ConetNode{0};
  short m_LinkNumber{0};
  static std::unordered_map<std::string,int> m_ModelList;
};

}
#endif
