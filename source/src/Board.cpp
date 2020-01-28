#include "Board.hpp"
#include "parser.hpp"
#include "literal.hpp"
#include "serializer.hpp"
#include "get.hpp"
#include "value.hpp"

ConnectorFactory Board::m_ConnectorFactory=ConnectorFactory();

Board::Board(const std::string& type,const std::string& name):Module(type,name)
{
  
}

void Board::DoDoConnect()
{
    m_Handle=m_Connector->Connect();
    DoConnect();
}

void Board::DoDoDisconnect()
{
  DoDisconnect();
  m_Connector->Disconnect();
}

void Board::DoInitialize()
{
  
}

void Board::DoConnect()
{
  
}

void Board::DoDisconnect()
{
  
}

void Board::LoadConfig()
{
  Module::LoadConfig();
  m_Connector=m_ConnectorFactory.createConnector(m_Config.getConnectorInfos(m_Name));
  m_Connector->verifyParameters();
}

void Board::printConnectorParameters()
{
  m_Connector->printParameters();
}
