#pragma once

#include "IXWebSocketServer.h"

#include "json/json.h"
#include <iostream>
#include <string>

class WebServer
{
public:
  WebServer(int port = ix::SocketServer::kDefaultPort, const std::string& host = ix::SocketServer::kDefaultHost, int backlog = ix::SocketServer::kDefaultTcpBacklog,
            std::size_t maxConnections = ix::SocketServer::kDefaultMaxConnections);
  void start() { m_server.start(); }
  void stop() { m_server.stop(); }
  void listen()
  {
    auto res = m_server.listen();
    if(!res.first)
    {
      // Error handling
      std::exit(1);
    }
  }

  void SendInfos(const std::string& name, const std::string& info);

  void SendStatus(const std::string& name);

  ix::WebSocketServer& Ref() { return m_server; }

  std::string Command() { return command; }

  void resetCommand() { command = ""; }

  std::string getStatus() { return status; }

  std::string getInfos(const std::string& key) { return Infos[key].asString(); }

  bool isStartedd() { return isStarted; }

private:
  ix::WebSocketServer m_server;
  std::string         command{""};
  std::string         status{"OPENED"};
  bool                isStarted{false};
  Json::Value         Infos;
};
