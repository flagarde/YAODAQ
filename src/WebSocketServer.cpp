#include "WebSocketServer.hpp"

#include "ixwebsocket/IXNetSystem.h"

#include "Exception.hpp"
#include "StatusCode.hpp"

namespace yaodaq
{


int WebSocketServer::loop()
{
  logger()->info("Server started on host {0} port {1}", m_Host, m_Port);
  listen();
  start();
  onRaisingSignal();
  stop();
  return 0;
}

WebSocketServer::WebSocketServer(const std::string& name,const int& port, const std::string& host, const int& backlog,const std::size_t& maxConnections, const int& handshakeTimeoutSecs,const int& addressFamily, const std::string& type,const CLASS& _class) : MessageHandlerServer(port,host,backlog,maxConnections,handshakeTimeoutSecs,addressFamily,Identifier(_class,type,name)), m_Host(host), m_Port(port)
{
  ix::initNetSystem();
  setOnClientMessageCallback(getMessageCallback());
  AddMethod("getClients", GetHandle(&WebSocketServer::getClientsIdentifier, *this), {});
}

WebSocketServer::~WebSocketServer()
{
  stop();
  ix::uninitNetSystem();
}

int WebSocketServer::getNumberOfClients()
{
  return m_Clients.getNumberOfClients();
}

std::vector<std::string> WebSocketServer::getClientsIdentifier()
{
  return m_Clients.getClientsIdentifier();
}

void WebSocketServer::listen()
{
  std::pair<bool, std::string> res = ix::WebSocketServer::listen();
  if(!res.first) { Exception(StatusCode::FAILURE, res.second); }
}

};
