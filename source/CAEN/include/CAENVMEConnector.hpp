#ifndef CAENVMECONNECTOR_HPP
#define CAENVMECONNECTOR_HPP
#include <string>
#include "Connector.hpp"

namespace CAEN
{

class CAENVMEConnector : public Connector
{ 
public:
  CAENVMEConnector();
  virtual int Connect() final;
  virtual void Disconnect() final;
private:
  virtual void verifyConfig() final;
  std::string m_Model{""};
};

}
#endif
