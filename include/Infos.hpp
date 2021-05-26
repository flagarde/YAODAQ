#pragma once

#include <string>

enum class Category
{
  Module,
  Board,
  UNKNOWN,
};

class Infos
{
public:
  explicit Infos(const std::string& roomName, const std::string& rackName, const std::string& crateName, const std::string& name, const std::string& type,const Category& category);
  explicit Infos(const std::string& id,const std::string& key);
  void        setRoomIndex(const int&);
  void        setRackIndex(const int&);
  void        setCrateIndex(const int&);
  void        setIndex(const int&);
  void        setKey(const std::string& key);
  std::string getRoomName() const;
  std::string getRackName() const;
  std::string getCrateName() const;
  std::string getName() const;
  std::string getType() const;
  int         getRoomIndex() const;
  int         getRackIndex() const;
  int         getCrateIndex() const;
  int         getIndex() const;
  std::string getKey() const;
  std::string getCategory() const;
  bool        operator<(const Infos& infos) const;
  Infos() = default;
  bool isA(const Category&);
  std::string getID() const;

private:
  std::string m_RoomName;
  int         m_RoomIndex{-1};
  std::string m_RackName;
  int         m_RackIndex{-1};
  std::string m_CrateName;
  int         m_CrateIndex{-1};
  std::string m_Name;
  int         m_Index{-1};
  std::string m_Type;
  Category    m_Category{Category::UNKNOWN};
  std::string m_ID;
};
