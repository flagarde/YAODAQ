#ifndef CONNECTIONSTATE_HPP
#define CONNECTIONSTATE_HPP
#include <memory>
#include <string>
#include "IXConnectionState.h"

class ConnectionState : public ix::ConnectionState
{
public:
  void setType(const std::string& type);
  std::string getType();
  static std::shared_ptr<ix::ConnectionState> createConnectionState();
private:
  std::string m_Type{""};
};

#endif
