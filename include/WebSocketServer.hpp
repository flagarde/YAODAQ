#pragma once

#include "MessageHandlerServer.hpp"

#include "GeneralParameters.hpp"
#include "Classes.hpp"

#include <string>
#include <vector>

namespace yaodaq
{



class WebSocketServer : public MessageHandlerServer
{
public:
  WebSocketServer(const std::string& name,const int& port = GeneralParameters::getPort(),
                  const std::string& host = GeneralParameters::getHost(),
                  const int& backlog = ix::SocketServer::kDefaultTcpBacklog,
                  const std::size_t& maxConnections = ix::SocketServer::kDefaultMaxConnections,
                  const int& handshakeTimeoutSecs = ix::WebSocketServer::kDefaultHandShakeTimeoutSecs,
                  const int& addressFamily = ix::SocketServer::kDefaultAddressFamily,
                  const std::string& type="Default",
                  const CLASS& clas=CLASS::WebSocketServer);
  virtual ~WebSocketServer();
  void listen();
  int loop();

  std::vector<std::string> getClientsIdentifier();
  int getNumberOfClients();

private:
  int m_BrowserNumber{1};
  std::string m_Host;
  int m_Port;
};

};
