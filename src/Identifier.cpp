#include "Identifier.hpp"

#include "magic_enum.hpp"

namespace yaodaq
{

  Identifier::Identifier(const CLASS& clas,const std::string& type, const std::string& name) : m_Class(clas), m_Type(type), m_Name(name){}

  std::string Identifier::getClass()
  {
    return std::string(magic_enum::enum_name(m_Class));
  }

  std::string Identifier::getType()
  {
    return m_Type;
  }

  std::string Identifier::getName()
  {
    return m_Name;
  }

  CLASS Identifier::getClassValue()
  {
    return m_Class;
  }

  std::string Identifier::getIdentifier()
  {
    return getClass()+"/"+getType()+"/"+getName();
  }

};
