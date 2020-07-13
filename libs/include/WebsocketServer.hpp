#pragma once

#include "IXWebSocketServer.h"
#include "Infos.hpp"

#include <map>
#include <string>

class WebsocketServer : public ix::WebSocketServer
{
public:
  WebsocketServer(const int& port = ix::SocketServer::kDefaultPort, const std::string& host = ix::SocketServer::kDefaultHost, const int& backlog = ix::SocketServer::kDefaultTcpBacklog,
                  const std::size_t& maxConnections = ix::SocketServer::kDefaultMaxConnections, const int& handshakeTimeoutSecs = ix::WebSocketServer::kDefaultHandShakeTimeoutSecs,const int& addressFamily = ix::SocketServer::kDefaultAddressFamily);
  ~WebsocketServer();
  void        setVerbosity(const std::string&);
  void        listen();
  std::string getkey(const std::shared_ptr<ix::WebSocket>& websocket);

private:
  std::map<Infos, std::shared_ptr<ix::WebSocket>> m_Clients;
  void                                            erase(const std::shared_ptr<ix::WebSocket>& socket);
  void                                            try_emplace(const std::string& key, const std::shared_ptr<ix::WebSocket>& socket);
  void                                            sendToLogger(const std::string& message);
  void                                            sendToAll(const std::string& message);
  static int                                      m_BrowserNumber;
  std::shared_ptr<ix::WebSocket>                  m_Actual;
};
