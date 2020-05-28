#pragma once

#include "IXWebSocketServer.h"
#include "Infos.hpp"

#include <map>
#include <string>

class WebsocketServer
{
public:
  WebsocketServer(const int& port = ix::SocketServer::kDefaultPort, const std::string& host = ix::SocketServer::kDefaultHost, const int& backlog = ix::SocketServer::kDefaultTcpBacklog, const std::size_t& maxConnections = ix::SocketServer::kDefaultMaxConnections, const int& handshakeTimeoutSecs = 3);
  ~WebsocketServer();
  void        setVerbosity(const std::string&);
  void        start();
  void        stop();
  void        wait();
  void        listen();
  std::string getkey(const std::shared_ptr<ix::WebSocket>& websocket);

private:
  ix::WebSocketServer                             m_Server;
  std::map<Infos, std::shared_ptr<ix::WebSocket>> m_Clients;
  void                                            erase(const std::shared_ptr<ix::WebSocket>& socket);
  void                                            try_emplace(const std::string& key, const std::shared_ptr<ix::WebSocket>& socket);
  void                                            sendToLogger(const std::string& message);
  void                                            sendToAll(const std::string& message);
  static int                                      m_BrowserNumber;
  std::shared_ptr<ix::WebSocket>                  m_Actual;
};
