#pragma once

#include "ixwebsocket/IXWebSocketServer.h"

#include "MessageHandlerServer.hpp"

#include "GeneralParameters.hpp"
#include "Identifier.hpp"
#include "Interrupt.hpp"

#include <string>

#include "Clients.hpp"

namespace yaodaq
{

  enum class  CLASS;

  class WebSocketServer: public ix::WebSocketServer, public MessageHandlerServer
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
    void        listen();
    int loop();
  private:
    Identifier m_Identifier;
    Interrupt m_Interrupt;
    Clients m_Clients;
    void                                  sendToLogger(const std::string& message);
    void                                  sendToAll(const std::string& message);
    int                                   m_BrowserNumber{1};
    std::string m_Host;
    int m_Port;
    std::string m_Actual;
  };

};
