#include "ConnectionState.hpp"

void ConnectionState::setType(const std::string& type)
{
  m_Type=type;
}

void ConnectionState::setName(const std::string& name)
{
  m_Name=name;
}

std::string ConnectionState::getType()
{
  return m_Type;
}

std::string ConnectionState::getName()
{
  return m_Name;
}

std::shared_ptr<ix::ConnectionState> ConnectionState::createConnectionState()
{
  return std::make_shared<ConnectionState>();
}
