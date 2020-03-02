#include "Board.hpp"

#include "toml.hpp"

ConnectorFactory Board::m_ConnectorFactory = ConnectorFactory();

Board::Board(const std::string& name, const std::string& type): Module(name, type) {}

void Board::CallModuleConnect()
{
  m_Connector = m_ConnectorFactory.createConnector(m_Config.getConnectorInfos(m_Name));
  m_Handle    = m_Connector->Connect();
  DoConnect();
}

void Board::CallModuleDisconnect()
{
  DoDisconnect();
  m_Connector->Disconnect();
}

void Board::printConnectorParameters()
{
  m_Connector->printParameters();
}

void Board::DoConnect() {}

void Board::DoDisconnect() {}
