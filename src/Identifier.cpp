#include "Identifier.hpp"

#include "magic_enum.hpp"
#include <vector>
#include <string>
#include "Exception.hpp"
#include "StatusCode.hpp"
//FIXME try to find a mix between Infos and Identifier

namespace yaodaq
{

  bool Identifier::operator<(const Identifier& infos) const
  {
    return this->getName() < infos.getName();
  }

  Identifier::Identifier(const CLASS& clas,const std::string& type, const std::string& name) : m_Class(clas), m_Type(type), m_Name(name){}

  Identifier::Identifier(const std::string& key)
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
      m_Class  = magic_enum::enum_cast<CLASS>(result[3]).value();
      m_Type  = result[4];
      m_Name = result[5];
    }
    else if(result.size() == 3)
    {
      m_Class  = magic_enum::enum_cast<CLASS>(result[0]).value();
      m_Type  = result[1];
      m_Name = result[2];
    }
    else
    {
      throw Exception(StatusCode::WRONG_NUMBER_PARAMETERS, "Number of argument in key should be 3 or 6 ([RoomName/RackName/CrateName/]Class/Type/Name) !");
    }
  }

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
