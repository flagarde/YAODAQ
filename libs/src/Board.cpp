#include "Board.hpp"

#include "toml.hpp"

ConnectorFactory Board::m_ConnectorFactory = ConnectorFactory();

Board::Board(const std::string& name, const std::string& type): Module(name, type) {}

void Board::CallModuleConnect()
{
  if(m_IsConnected == false)
  {
    m_Connector = m_ConnectorFactory.createConnector(m_Config.getConnectorInfos(m_Name));
    m_Handle    = m_Connector->Connect();
    DoConnect();
    m_IsConnected = true;
  }
}

void Board::CallModuleDisconnect()
{
  if(m_IsConnected == true)
  {
    DoDisconnect();
    m_Connector->Disconnect();
    m_IsConnected = false;
  }
}

void Board::printConnectorParameters()
{
  m_Connector->printParameters();
}

void Board::DoConnect() {}

void Board::DoDisconnect() {}
