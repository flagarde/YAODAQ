#pragma once

#include "WebsocketServer.hpp"
#include "soci/session.h"

#include <string>

class Configurator: public WebsocketServer
{
public:
  Configurator(const int& port = ix::SocketServer::kDefaultPort, const std::string& host = ix::SocketServer::kDefaultHost, const int& backlog = ix::SocketServer::kDefaultTcpBacklog,
               const std::size_t& maxConnections = ix::SocketServer::kDefaultMaxConnections, const int& handshakeTimeoutSecs = 3);

private:
  soci::session m_session;
};
