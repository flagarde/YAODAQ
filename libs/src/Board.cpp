#include "Board.hpp"

ConnectorFactory Board::m_ConnectorFactory = ConnectorFactory();

Board::Board(const std::string& name, const std::string& type): Module(name, type)
{
  m_ConnectorFactory.loadConnectors();
}

void Board::CallBoardConnect()
{
  if(m_IsConnected == false)
  {
    m_Connector = m_ConnectorFactory.createConnector(m_Config.getConnectorInfos(m_Name));
    m_Handle    = m_Connector->Connect();
    DoConnect();
    m_IsConnected = true;
  }
}

void Board::CallBoardDisconnect()
{
  std::cout << "Board Disconnect" << std::endl;
  if(m_IsConnected == true)
  {
    DoDisconnect();
    m_Connector->Disconnect();
    m_IsConnected = false;
  }
  std::cout << "Board Disconnect" << std::endl;
}

void Board::printConnectorParameters()
{
  m_Connector->printParameters();
}

void Board::DoConnect() {}

void Board::DoDisconnect() {}
