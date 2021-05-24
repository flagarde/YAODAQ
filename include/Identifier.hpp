#pragma once

#include <string>

#include "Classes.hpp"

namespace yaodaq
{

  class Identifier
  {
  public:
    Identifier(const CLASS&,const std::string&, const std::string&);
    std::string getClass() const;
    std::string getType() const;
    std::string getName() const;
    CLASS getClassValue() const;
    std::string getIdentifier() const;
  private:
    CLASS m_Class{CLASS::Unknown};
    std::string m_Type{"Unknown"};
    std::string m_Name{"Unnamed"};
  };

};
