#include "Board.hpp"

#include "toml.hpp"

ConnectorFactory Board::m_ConnectorFactory = ConnectorFactory();

Board::Board(const std::string& name, const std::string& type): Module(name, type) {}

void Board::DoDoConnect()
{
  m_Handle = m_Connector->Connect();
  DoConnect();
}

void Board::DoDoDisconnect()
{
  DoDisconnect();
  m_Connector->Disconnect();
}

void Board::DoInitialize() {}

void Board::DoConnect() {}

void Board::DoDisconnect() {}

void Board::printConnectorParameters()
{
  m_Connector->printParameters();
}
