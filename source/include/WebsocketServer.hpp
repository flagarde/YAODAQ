#ifndef WEBSOCKETSERVER_HPP
#define WEBSOCKETSERVER_HPP
#include <string>
#include "IXWebSocketServer.h"
#include "Message.hpp"

class WebsocketServer
{
public:
  WebsocketServer(const int& port = ix::SocketServer::kDefaultPort,const std::string& host = ix::SocketServer::kDefaultHost,const int& backlog = ix::SocketServer::kDefaultTcpBacklog,const std::size_t& maxConnections = ix::SocketServer::kDefaultMaxConnections,const int& handshakeTimeoutSecs = 3);
  ~WebsocketServer();
  void start();
  void stop();
  void wait();
  void listen();
private:
  ix::WebSocketServer m_Server;
};
#endif
