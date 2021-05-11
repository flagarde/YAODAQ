#pragma once

#include <string>

#include "Classes.hpp"

namespace yaodaq
{

  class Identifier
  {
  public:
    Identifier(const CLASS&,const std::string& type, const std::string& name);
    std::string getClass();
    std::string getType();
    std::string getName();
    CLASS getClassValue();
    std::string getIdentifier();
  private:
    CLASS m_Class{CLASS::Unknown};
    std::string m_Type{"Unknown"};
    std::string m_Name{"Unnamed"};
  };

};
