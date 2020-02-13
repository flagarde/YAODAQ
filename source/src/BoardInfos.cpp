#include "BoardInfos.hpp"
#include "toml.hpp"

BoardInfos::BoardInfos(const std::string& room,const std::string& rack,const std::string& crate,const std::string& board,const toml::value& parm,const toml::value& conparm)
{
  m_RoomName=room;
  m_RackName=rack;
  m_CrateName=crate;
  m_BoardName=board;
  m_ConnectorParameters=conparm;
  m_Parameters=parm;
}

void BoardInfos::setRoomIndex(const int& index)
{
  m_RoomIndex=index;
}

void BoardInfos::setRackIndex(const int& index)
{
  m_RackIndex=index;
}
  
void BoardInfos::setCrateIndex(const int& index)
{
  m_CrateIndex=index;
}
  
void BoardInfos::setBoardIndex(const int& index)
{
  m_BoardIndex=index; 
}
  
std::string BoardInfos::getRoomName()
{
  return m_RoomName; 
}
  
std::string BoardInfos::getRackName()
{
  return m_RackName;
}

std::string BoardInfos::getCrateName()
{
  return m_CrateName;
}

std::string BoardInfos::getBoardName()
{
  return m_BoardName;
}

int BoardInfos::getRoomIndex()
{
  return m_RoomIndex;
}
  
int BoardInfos::getRackIndex()
{
  return m_RackIndex;
}

int BoardInfos::getCrateIndex()
{
  return m_CrateIndex;
}  

int BoardInfos::getBoardIndex()
{
  return m_BoardIndex;
}

void BoardInfos::print(std::ostream& os, const std::string shift)
{
  os<<shift<<"Board "<<m_BoardIndex<<" : "<<m_BoardName<<std::endl;
  os<<shift<<"Room "<<m_RoomIndex<<" : "<<m_RoomName<<" ;"<<std::endl;
  os<<shift<<"Rack "<<m_RackIndex<<" : "<<m_RackName<<" ;"<<std::endl;
  os<<shift<<"Crate "<<m_CrateIndex<<" : "<<m_CrateName<<" ;"<<std::endl;
  os<<shift<<"Parameters :"<<std::endl;
  os<<shift<<m_Parameters<<std::endl;
  os<<shift<<"Connector Parameters :"<<std::endl;
  os<<shift<<m_ConnectorParameters<<std::endl;
}
