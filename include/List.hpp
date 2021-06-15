#pragma once

#include <string>
#include <set>

namespace Json
{
  class Value;
}

namespace yaodaq
{

class List
{
public:
  void update(const Json::Value& value,const std::string& me)
  {
    for(Json::Value::const_iterator it=value.begin(); it!=value.end(); ++it)
    {
      if(it->asString()!=me) m_List.insert(it->asString());
      std::cout<<"777777777"<<it->asString()<<std::endl;
    }
  }

  bool has(const std::string& toto)
  {
    if(m_List.find(toto)!=m_List.end()) return true;
    else return false;
  }

  std::size_t size()
  {
    return m_List.size();
  }

  void erase(const std::string& key)
  {
    m_List.erase(key);
  }

  void clear()
  {
    m_List.clear();
  }
private:
  std::set<std::string> m_List;

};

};
