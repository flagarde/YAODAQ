#include "Connector.hpp"

Connector::Connector(const std::string& type): m_Type(type) {}

toml::value Connector::getParameters()
{
  return m_Infos.getParameters();
}

void Connector::printParameters()
{
  m_Infos.printParameters();
}

std::int32_t Connector::Connect()
{
  if(!isConnected())
  {
    verifyParameters();
    DoConnect();
    m_Infos.setHandle(m_Handle);
  }
  m_Infos.addBoardConnected();
  return m_Handle;
}

void Connector::setInfos(const ConnectorInfos& infos)
{
  m_Infos = infos;
}

std::string Connector::getType()
{
  return m_Type;
}

void Connector::Disconnect()
{
  m_Infos.removeBoardConnected();
  if(!isConnected()) DoDisconnect();
}

void Connector::verifyParameters() {}

bool Connector::isConnected()
{
  if(m_Infos.getNumberBoardConnected() == 0) return false;
  else return true;
} 
