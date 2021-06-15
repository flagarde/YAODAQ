#include "Board.hpp"
#include "Exception.hpp"

namespace yaodaq
{

ConnectorFactory Board::m_ConnectorFactory = ConnectorFactory();

Board::Board(const std::string& name, const std::string& type, const yaodaq::CLASS& _class): Module(name, type, _class)
{
  try
  {
    m_ConnectorFactory.loadConnectors();
  }
  catch(const Exception& exception)
  {
    error(exception.what());
    stopListening();
    throw;
  }
}

void Board::CallBoardConnect()
{
  if(m_IsConnected == false)
  {
    m_Config.reloadConnectorParameters(getIdentifier().getName());
    m_Connector = m_ConnectorFactory.createConnector(m_Config.getConnectorInfos(getIdentifier().getName()));
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

std::shared_ptr<Connector> Board::getConnector()
{
  if(m_Connector!=nullptr) return m_Connector;
  else
  {
    error("{} need to be connected before sending to connector !",getName());
    return nullptr;
  }
}

void Board::DoConnect() {}

void Board::DoDisconnect() {}

};
