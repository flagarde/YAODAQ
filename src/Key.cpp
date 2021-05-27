#include "Key.hpp"

namespace yaodaq
{

void Key::setRoom(const std::string& room)
{
  m_Args[ROOM] = room;
}

Key& Key::Room(const std::string& room)
{
  setRoom(room);
  return *this;
}

std::string Key::getRoom()
{
  return m_Args[ROOM];
}

void Key::setRack(const std::string& rack)
{
  m_Args[RACK] = rack;
}

Key& Key::Rack(const std::string& rack)
{
  setRack(rack);
  return *this;
}

std::string Key::getRack()
{
  return m_Args[RACK];
}

void Key::setCrate(const std::string& crate)
{
  m_Args[CRATE] = crate;
}

Key& Key::Crate(const std::string& crate)
{
  setCrate(crate);
  return *this;
}

std::string Key::getCrate()
{
  return m_Args[CRATE];
}

void Key::setSlot(const std::string& slot)
{
  m_Args[SLOT] = slot;
}

Key& Key::Slot(const std::string& slot)
{
  setSlot(slot);
  return *this;
}

std::string Key::getSlot()
{
  return m_Args[SLOT];
}

void Key::setClass(const std::string& Class)
{
  m_Args[CLASS] = Class;
}

Key& Key::Class(const std::string& Class)
{
  setClass(Class);
  return *this;
}

std::string Key::getClass()
{
  return m_Args[CLASS];
}

void Key::setType(const std::string& type)
{
  m_Args[TYPE] = type;
}

Key& Key::Type(const std::string& type)
{
  setType(type);
  return *this;
}

std::string Key::getType()
{
  return m_Args[TYPE];
}

void Key::setName(const std::string& name)
{
  m_Args[NAME] = name;
}

Key& Key::Name(const std::string& name)
{
  setName(name);
  return *this;
}

std::string Key::getName()
{
  return m_Args[NAME];
}

std::string Key::getKey()
{
  createkey();
  return m_Key.data();
}

void Key::createkey()
{
  fmt::format_to(m_Key, "{ROOM}/{RACK}/{CRATE}/{SLOT}/{CLASS}/{TYPE}/{NAME}", m_Args[ROOM], m_Args[RACK], m_Args[CRATE], m_Args[SLOT], m_Args[CLASS], m_Args[TYPE], m_Args[NAME]);
};

};
