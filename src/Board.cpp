#include "Board.hpp"
#include "Exception.hpp"

ConnectorFactory Board::m_ConnectorFactory = ConnectorFactory();

Board::Board(const std::string& name, const std::string& type): Module(name, type)
{
  try
  {
    m_ConnectorFactory.loadConnectors();
  }
  catch(const Exception& error)
  {
    sendError(error.what());
    stopListening();
    throw;
  }
}

void Board::CallBoardConnect()
{
  if(m_IsConnected == false)
  {
    m_Config.reloadConnectorParameters(m_Name);
    m_Connector = m_ConnectorFactory.createConnector(m_Config.getConnectorInfos(m_Name));
    m_Handle    = m_Connector->Connect();
    DoConnect();
    m_IsConnected = true;
  }
}

void Board::CallBoardDisconnect()
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
