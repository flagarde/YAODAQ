#include "Infos.hpp"

#include "Exception.hpp"
#include "StatusCode.hpp"
#include "magic_enum.hpp"

#include <vector>

Infos::Infos(const std::string& roomName, const std::string& rackName, const std::string& crateName, const std::string& name, const std::string& type,const Category& category)
{
  m_RoomName  = roomName;
  m_RackName  = rackName;
  m_CrateName = crateName;
  m_Name      = name;
  m_Type      = type;
  m_Category  = category;
}

std::string Infos::getID() const
{
  return m_ID;
}

std::string Infos::getCategory() const
{
  return std::string(magic_enum::enum_name(m_Category));
}

bool Infos::isA(const Category& categorie)
{
  if(categorie==m_Category) return true;
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
    else
      result.push_back("");
    tmp        = tmp.substr(second_pos + separator.length());
    second_pos = tmp.find(separator);
    if(second_pos == std::string::npos) result.push_back(tmp);
  }
  if(result.size() == 5)
  {
    m_RoomName  = result[0];
    m_RackName  = result[1];
    m_CrateName = result[2];
    m_Type      = result[3];
    m_Name      = result[4];
  }
  else
  {
    throw Exception(StatusCode::WRONG_NUMBER_PARAMETERS, "Number of argument in key should be 5 (RoomName/RackName/CrateName/Type/Name) !");
  }
}

std::string Infos::getKey() const
{
  return m_RoomName + "/" + m_RackName + "/" + m_CrateName + "/" + m_Type + "/" + m_Name;
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
  return m_Name;
}

std::string Infos::getType() const
{
  return m_Type;
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
