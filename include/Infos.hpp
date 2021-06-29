#pragma once

#include <string>

#include "Classes.hpp"
#include "Identifier.hpp"

namespace yaodaq
{

class Infos
{
public:
  explicit Infos(const std::string& roomName, const std::string& rackName, const std::string& crateName,const int& slot,const CLASS& _class,const std::string& type, const std::string& name);
  explicit Infos(const std::string& id,const std::string& identifier);
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
  std::string getClass() const;
  std::string getJson() const;
  bool        operator<(const Infos& infos) const;
  Infos() = default;
  bool isA(const CLASS&) const;
  bool isWebBased() const;
  std::string getID() const;
  Identifier getIdentifier() const;
private:
  std::string m_RoomName;
  int         m_RoomIndex{-1};
  std::string m_RackName;
  int         m_RackIndex{-1};
  std::string m_CrateName;
  int         m_CrateIndex{-1};
  int         m_Slot{-1};
  int         m_Index{-1};
  std::string m_ID;
  Identifier m_Identifier;
  std::string m_Json;
};

}
