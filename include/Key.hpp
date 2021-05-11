#pragma once

#include "fmt/format.h"

#include <array>
#include <string>

class Key
{
public:
  void        setRoom(const std::string& room);
  Key&        Room(const std::string& room);
  std::string getRoom();
  void        setRack(const std::string& rack);
  Key&        Rack(const std::string& rack);
  std::string getRack();
  void        setCrate(const std::string& crate);
  Key&        Crate(const std::string& crate);
  std::string getCrate();
  void        setSlot(const std::string& slot);
  Key&        Slot(const std::string& slot);
  std::string getSlot();
  void        setClass(const std::string& Class);
  Key&        Class(const std::string& Class);
  std::string getClass();
  void        setType(const std::string& type);
  Key&        Type(const std::string& type);
  std::string getType();
  void        setName(const std::string& name);
  Key&        Name(const std::string& name);
  std::string getName();
  std::string getKey();

private:
  enum Keys
  {
    ROOM,
    RACK,
    CRATE,
    SLOT,
    CLASS,
    TYPE,
    NAME
  };
  void                       createkey();
  std::array<std::string, 6> m_Args;
  fmt::memory_buffer         m_Key;
};
