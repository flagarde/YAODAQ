#pragma once

#include "IXWebSocketCloseConstants.h"
#include "IXWebSocketServer.h"
#include "Infos.hpp"
#include "json.h"
#include "spdlog.h"

#include <string>
#include <unordered_map>

class WebsocketServer
{
public:
  WebsocketServer(const int& port = ix::SocketServer::kDefaultPort, const std::string& host = ix::SocketServer::kDefaultHost,
                  const int&         backlog        = ix::SocketServer::kDefaultTcpBacklog,
                  const std::size_t& maxConnections = ix::SocketServer::kDefaultMaxConnections, const int& handshakeTimeoutSecs = 3);
  ~WebsocketServer();
  void setVerbosity(const std::string&);
  void start();
  void stop();
  void wait();
  void listen();

private:
  ix::WebSocketServer                             m_Server;
  std::map<Infos, std::shared_ptr<ix::WebSocket>> m_Clients;
  void                                            erase(const std::shared_ptr<ix::WebSocket>& socket);
  spdlog::level::level_enum                       m_Verbosity{spdlog::level::trace};
  void                                            try_emplace(const std::string& key, const std::shared_ptr<ix::WebSocket>& socket);
  void                                            sendToLogger(const std::string& message);
  void                                            sendToAll(const std::string& message);
  std::mutex                                      m_Mutex;
  static int                                      m_BrowserNumber;
  std::shared_ptr<ix::WebSocket>                  m_Actual;
};
