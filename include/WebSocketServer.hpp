#pragma once

#include "ixwebsocket/IXWebSocketServer.h"
#include "spdlog/fwd.h"

#include "GeneralParameters.hpp"
#include "Identifier.hpp"
#include "Interrupt.hpp"

#include <string>

#include "Clients.hpp"

namespace yaodaq
{

  enum class  CLASS;

  class WebSocketServer: public ix::WebSocketServer
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
    ~WebSocketServer();
    void        setVerbosity(const std::string&);
    void        listen();
    int loop();
  private:
    void signalMessage();
    Identifier m_Identifier;
    Interrupt m_Interrupt;
    Clients m_Clients;
    void                                  sendToLogger(const std::string& message);
    void                                  sendToAll(const std::string& message);
    int                                   m_BrowserNumber{1};
    std::string                           m_Actual;
    std::shared_ptr<spdlog::logger> m_Logger{nullptr};
    std::string m_Host;
    int m_Port;
  };

};
