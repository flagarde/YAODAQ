#ifndef BOARDINFOS_HPP
#define BOARDINFOS_HPP
#include <string>
#include <iostream>
#include "value.hpp"

class BoardInfos 
{
public:
  BoardInfos(const std::string& room="",const std::string& rack="",const std::string& crate="",const std::string& board="",const toml::value& parm={},const toml::value& conparm={});
  void setRoomIndex(const int&);
  void setRackIndex(const int&);
  void setCrateIndex(const int&);
  void setBoardIndex(const int&);
  std::string getRoomName();
  std::string getRackName();
  std::string getCrateName();
  std::string getBoardName();
  int getRoomIndex();
  int getRackIndex();
  int getCrateIndex();
  int getBoardIndex();
  void print(std::ostream&  os=std::cout, const std::string shift="");
private:
  std::string m_RoomName{""};
  int m_RoomIndex{-1};
  std::string m_RackName{""};
  int m_RackIndex{-1};
  std::string m_CrateName{""};
  int m_CrateIndex{-1};
  std::string m_BoardName{""};
  int m_BoardIndex{-1};
  toml::value m_ConnectorParameters;
  toml::value m_Parameters;
};

#endif
