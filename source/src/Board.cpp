#include "Board.hpp"

Board::Board(const std::string& type,const std::string& name):Module(type,name)
{
  
}

void Board::Connect()
{
  m_Handle=m_Connector->Connect();
}

void Board::Disconnect()
{
  m_Connector->Disconnect();
}
