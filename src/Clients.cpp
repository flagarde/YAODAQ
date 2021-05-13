#include "Clients.hpp"

#include "Exception.hpp"
#include "StatusCode.hpp"

namespace yaodaq
{

void Clients::erase(const std::string& id)
{
  for(std::map<Infos, ix::WebSocket&>::iterator it=m_Clients.begin(); it!=m_Clients.end();++it)
  {
    if(it->first.getID()==id)
    {
      m_Clients.erase(it);
      break;
    }
  }
}

const Infos& Clients::getInfos(const std::string& id) const
{
  std::map<Infos, ix::WebSocket&>::const_iterator found;
  for(std::map<Infos, ix::WebSocket&>::const_iterator it=m_Clients.begin(); it!=m_Clients.end();++it)
  {
    if(it->first.getID()==id)
    {
      found=it;
      break;
    }
  }
  return found->first;
}

void Clients::try_emplace(const std::string& id, const std::string& key, ix::WebSocket& socket)
{
  for(std::map<Infos, ix::WebSocket&>::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it)
  {
    if(it->first.getKey() == key)
    {
      throw Exception(StatusCode::ALREADY_PRESENT,"The Name \"{}\" is already taken so it cannot be connected !",key);
    }
  }
  m_Clients.try_emplace(Infos(id,key),socket);
}

std::map<Infos, ix::WebSocket&>::iterator Clients::begin()
{
  return m_Clients.begin();
}

std::map<Infos, ix::WebSocket&>::iterator Clients::end()
{
  return m_Clients.end();
}

}
