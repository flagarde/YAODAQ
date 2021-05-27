#include "Identifier.hpp"

#include "magic_enum.hpp"

namespace yaodaq
{

  Identifier::Identifier(const CLASS& clas,const std::string& type, const std::string& name) : m_Class(clas), m_Type(type), m_Name(name){}

  std::string Identifier::getClassStr() const
  {
    return std::string(magic_enum::enum_name(m_Class));
  }

  std::string Identifier::getType() const
  {
    return m_Type;
  }

  std::string Identifier::getName() const
  {
    return m_Name;
  }

  CLASS Identifier::getClass() const
  {
    return m_Class;
  }

  std::string Identifier::get() const
  {
    return getClassStr()+"/"+getType()+"/"+getName();
  }

};
