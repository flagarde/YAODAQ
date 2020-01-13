#include "ConnectionState.hpp"

void ConnectionState::setType(const std::string& type)
{
  m_Type=type;
}

std::string ConnectionState::getType()
{
  return m_Type;
}

std::shared_ptr<ix::ConnectionState> ConnectionState::createConnectionState()
{
  return std::make_shared<ConnectionState>();
}
