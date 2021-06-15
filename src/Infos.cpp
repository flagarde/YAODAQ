#include "Infos.hpp"

#include "Exception.hpp"
#include "StatusCode.hpp"
#include "magic_enum.hpp"

#include <vector>

#include <iostream>

namespace yaodaq
{

Infos::Infos(const std::string& roomName, const std::string& rackName, const std::string& crateName, const int& slot, const CLASS& _class,const std::string& type, const std::string& name)
{
  m_RoomName  = roomName;
  m_RackName  = rackName;
  m_CrateName = crateName;
  m_Identifier = Identifier(_class,type,name);
  m_Slot      = slot;
}

std::string Infos::getID() const
{
  return m_ID;
}

std::string Infos::getClass() const
{
  return m_Identifier.getClassStr();
}

bool Infos::isA(const CLASS& _class) const
{
  if(m_Identifier.getClass()==_class) return true;
  else return false;
}

Infos::Infos(const std::string& id,const std::string& key) : m_ID(id)
{
  setKey(key);
}

bool Infos::operator<(const Infos& infos) const
{
  return this->getID() < infos.getID();
}

bool Infos::isWebBased() const
{
  if(magic_enum::enum_integer(m_Identifier.getClass())>=magic_enum::enum_integer(CLASS::Web)) return true;
  else return false;
}

Identifier Infos::getIdentifier() const
{
  return m_Identifier;
}

void Infos::setKey(const std::string& key)
{
  std::vector<std::string> result;
  std::string              tmp        = key;
  std::string              separator  = "/";
  std::size_t              second_pos = tmp.find(separator);
  while(second_pos != std::string::npos)
  {
    if(0 != second_pos)
    {
      std::string word = tmp.substr(0, second_pos - 0);
      result.push_back(word);
    }
    else result.push_back("");
    tmp        = tmp.substr(second_pos + separator.length());
    second_pos = tmp.find(separator);
    if(second_pos == std::string::npos) result.push_back(tmp);
  }
  if(result.size() == 6)
  {
    m_RoomName  = result[0];
    m_RackName  = result[1];
    m_CrateName = result[2];
    m_Identifier= Identifier(magic_enum::enum_cast<CLASS>(result[3]).value(),result[4],result[5]);
  }
  else
  {
    throw Exception(StatusCode::WRONG_NUMBER_PARAMETERS, "Number of argument in key should be 6 (RoomName/RackName/CrateName/Class/Type/Name) !");
  }
}

std::string Infos::getKey() const
{
  return m_RoomName + "/" + m_RackName + "/" + m_CrateName + "/"+ getClass() + "/" + getType() + "/" + getName();
}

void Infos::setRoomIndex(const int& index)
{
  m_RoomIndex = index;
}

void Infos::setRackIndex(const int& index)
{
  m_RackIndex = index;
}

void Infos::setCrateIndex(const int& index)
{
  m_CrateIndex = index;
}

void Infos::setIndex(const int& index)
{
  m_Index = index;
}

std::string Infos::getRoomName() const
{
  return m_RoomName;
}

std::string Infos::getRackName() const
{
  return m_RackName;
}

std::string Infos::getCrateName() const
{
  return m_CrateName;
}

std::string Infos::getName() const
{
  return m_Identifier.getName();
}

std::string Infos::getType() const
{
  return m_Identifier.getType();
}

int Infos::getRoomIndex() const
{
  return m_RoomIndex;
}

int Infos::getRackIndex() const
{
  return m_RackIndex;
}

int Infos::getCrateIndex() const
{
  return m_CrateIndex;
}

int Infos::getIndex() const
{
  return m_Index;
}

};
