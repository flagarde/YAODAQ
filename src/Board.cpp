#include "Board.hpp"
#include "Exception.hpp"
#include "StatusCode.hpp"

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
    m_Connector = std::move(m_ConnectorFactory.createConnector(m_Config.getConnectorInfos(getIdentifier().getName())));
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
    m_Connector = std::make_shared<DumpConnector>();
  }
}

void Board::printConnectorParameters()
{
  m_Connector->printParameters();
}

const std::shared_ptr<Connector>& Board::getConnector()
{
  if(m_Connector->getType()=="DumpConnector")
  {
    error("Connector for {} is not connected, sending everything to DumpConnector.",getName());
  }
  return m_Connector;
}

void Board::DoConnect() {}

void Board::DoDisconnect() {}

}
