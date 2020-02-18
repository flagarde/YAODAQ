#ifndef CONNECTIONSTATE_HPP
#define CONNECTIONSTATE_HPP

#include <memory>
#include <string>
#include "IXConnectionState.h"

class ConnectionState : public ix::ConnectionState
{
public:
  void setType(const std::string& type);
  void setName(const std::string& name);
  std::string getType();
  std::string getName();
  static std::shared_ptr<ix::ConnectionState> createConnectionState();
private:
  std::string m_Type{""};
  std::string m_Name{""};
};

#endif
