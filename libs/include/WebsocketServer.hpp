#pragma once

#include "IXWebSocketServer.h"
#include "Infos.hpp"
#include "json.h"
#include "spdlog.h"

#include <string>
#include <unordered_map>

class WebsocketServer
{
public:
  WebsocketServer(const int&         port = ix::SocketServer::kDefaultPort,
                  const std::string& host = ix::SocketServer::kDefaultHost,
                  const int& backlog = ix::SocketServer::kDefaultTcpBacklog,
                  const std::size_t& maxConnections =
                      ix::SocketServer::kDefaultMaxConnections,
                  const int& handshakeTimeoutSecs = 3);
  ~WebsocketServer();
  void setVerbosity(const std::string&);
  void start();
  void stop();
  void wait();
  void listen();

private:
  ix::WebSocketServer                             m_Server;
  std::map<Infos, std::shared_ptr<ix::WebSocket>> m_Clients;
  Infos getInfos(const std::shared_ptr<ix::WebSocket>& socket);
  void  erase(const std::shared_ptr<ix::WebSocket>& socket);
  spdlog::level::level_enum m_Verbosity{spdlog::level::warn};
};
