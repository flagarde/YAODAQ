#include "Board.hpp"
#include "parser.hpp"
#include "literal.hpp"
#include "serializer.hpp"
#include "get.hpp"

Board::Board(const std::string& type,const std::string& name):Module(type,name)
{
  
}

void Board::DoInitialize()
{
  
  
}

void Board::DoConnect()
{
  m_Handle=m_Connector->Connect();
}

void Board::DoDisconnect()
{
  m_Connector->Disconnect();
}

void Board::LoadConfig()
{
  Module::LoadConfig();
  m_ConnectorConf=m_Config.getConnectorConfig(m_Name);
  if(m_Connector!=nullptr) m_Connector->setConfiguration(m_ConnectorConf);
}

void Board::printConnectorParameters()
{
  std::cout<<m_ConnectorConf<<std::endl;
}
