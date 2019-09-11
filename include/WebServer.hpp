#ifndef WEBSERVER_h
#define WEBSERVER_h
#include "IXWebSocketServer.h"
#include <string>

class WebServer
{
public:
 WebServer(int port = ix::SocketServer::kDefaultPort,
                     const std::string& host = ix::SocketServer::kDefaultHost,
                     int backlog = ix::SocketServer::kDefaultTcpBacklog,
                     std::size_t maxConnections = ix::SocketServer::kDefaultMaxConnections);
 void start(){ m_server.start();}
 void stop(){m_server.stop();}
 void listen(){


auto res = m_server.listen();
if (!res.first)
{
    // Error handling
   std::exit(1);
}

 }

 ix::WebSocketServer& Ref()
 {

   return m_server;
 }


 std::string Command()
 {
	return command;
 }
  
 void resetCommand()
 {

    command="Where";
 }
private:
  ix::WebSocketServer m_server;
  std::string command{"Where"};
};
#endif
