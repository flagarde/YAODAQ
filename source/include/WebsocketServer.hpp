#ifndef WEBSOCKETSERVER_HPP
#define WEBSOCKETSERVER_HPP
#include <string>
#include "IXWebSocketServer.h"

class WebsocketServer
{
public:
  WebsocketServer(const int& port,const std::string& host,const int& backlog,const std::size_t& maxConnections);
  ~WebsocketServer();
  void Start();
  void Stop();
  void Wait();
  void Listen();
private:
  ix::WebSocketServer m_Server;
};
#endif
