#pragma once

#include <string>
#include <map>
#include <vector>

#include "Infos.hpp"

namespace ix
{
  class WebSocket;
};

namespace yaodaq
{

class Clients
{
public:
  void erase(const std::string& id);
  void try_emplace(const std::string& id, const std::string& key, ix::WebSocket& socket);
  std::map<Infos, ix::WebSocket&>::iterator begin();
  std::map<Infos, ix::WebSocket&>::iterator end();
  const Infos& getInfos(const std::string& id) const;
  std::size_t getNumberOfClients() const;

  std::vector<std::string> getClientsIdentifier();

private:

  std::map<Infos, ix::WebSocket&>       m_Clients;
};

};
