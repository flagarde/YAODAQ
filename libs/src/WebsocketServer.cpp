#include "WebsocketServer.hpp"

#include "Exception.hpp"
#include "IXNetSystem.h"
#include "Message.hpp"
#include "json.h"
#include "spdlog.h"

#include <iostream>
void WebsocketServer::setVerbosity(const std::string& verbosity)
{
  if(verbosity == "off") m_Verbosity = spdlog::level::off;
  else if(verbosity == "trace")
    m_Verbosity = spdlog::level::trace;
  else if(verbosity == "info")
    m_Verbosity = spdlog::level::info;
  else if(verbosity == "debug")
    m_Verbosity = spdlog::level::debug;
  else if(verbosity == "warning")
    m_Verbosity = spdlog::level::warn;
  else if(verbosity == "critical")
    m_Verbosity = spdlog::level::critical;
  spdlog::set_level(m_Verbosity);
}

WebsocketServer::WebsocketServer(const int& port, const std::string& host, const int& backlog, const std::size_t& maxConnections,
                                 const int& handshakeTimeoutSecs)
    : m_Server(port, host, backlog, maxConnections, handshakeTimeoutSecs)
{
  ix::initNetSystem();
  m_Server.setOnConnectionCallback([this](std::shared_ptr<ix::WebSocket> webSocket, std::shared_ptr<ix::ConnectionState> connectionState) {
    webSocket->setOnMessageCallback([webSocket, connectionState, this](const ix::WebSocketMessagePtr& msg) {
      static int browserNumber = 1;
      if(msg->type == ix::WebSocketMessageType::Open)
      {
        std::string key = "///";
        // Print in webserver console
        spdlog::info("New connection ID : {}", connectionState->getId());
        spdlog::info("Uri : {}", msg->openInfo.uri);
        spdlog::info("Headers :");
        for(auto it: msg->openInfo.headers) { spdlog::info("\t{} : {}", it.first, it.second); }
        //
        if(msg->openInfo.headers.find("Key") != msg->openInfo.headers.end()) { key = msg->openInfo.headers["Key"]; }
        else
        {
          key += "Browser/Browser" + std::to_string(browserNumber);
          ++browserNumber;
        }

        // Error coming from Error here should not crash the Webserver but
        // disconnect the websocket of the bad guy !" Look if the name is
        // already taken an put an erro if it is
        try_emplace(key, webSocket);
        // Print in all the Logger consoles
        for(std::map<Infos, std::shared_ptr<ix::WebSocket>>::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it)
        {
          if(it->first.getType() == "Logger")
          {
            it->second->send("New connection ID : " + connectionState->getId());
            it->second->send("Uri :  : " + msg->openInfo.uri);
            it->second->send("Headers :");
            for(auto itt: msg->openInfo.headers) { it->second->send("\t" + itt.first + " : " + itt.second); }
          }
        }
      }
      else if(msg->type == ix::WebSocketMessageType::Close)
      {
        spdlog::info("Closed connection ID : {}", connectionState->getId());
        //
        for(std::map<Infos, std::shared_ptr<ix::WebSocket>>::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it)
        {
          if(it->first.getType() == "Logger" && webSocket != it->second) { it->second->send("Closed connection ID : " + connectionState->getId()); }
        }
        erase(webSocket);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
      else if(msg->type == ix::WebSocketMessageType::Message)
      {
        spdlog::info("{}", msg->str);
        Message message;
        try
        {
          message.parse(msg->str);
        }
        catch(...)
        {
          for(std::map<Infos, std::shared_ptr<ix::WebSocket>>::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it)
          {
            if(it->second != webSocket) it->second->send(msg->str);
          }
        }
        // If it's Log only send to Loggers :)
        if(message.getType() == "Log")
        {
          for(std::map<Infos, std::shared_ptr<ix::WebSocket>>::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it)
          {
            if(it->second != webSocket && it->first.getType() == "Logger") { it->second->send(msg->str); }
          }
        }
        else if(message.getType() == "Log")
        {
        }
        else
        {
          for(std::map<Infos, std::shared_ptr<ix::WebSocket>>::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it)
          {
            if(it->second != webSocket) it->second->send(msg->str);
          }
        }
      }
    });
  });
}

Infos WebsocketServer::getInfos(const std::shared_ptr<ix::WebSocket>& socket)
{
  for(std::map<Infos, std::shared_ptr<ix::WebSocket>>::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it)
  {
    if(it->second == socket) return it->first;
  }
  // FIX ME a better one;
  throw;
}

void WebsocketServer::erase(const std::shared_ptr<ix::WebSocket>& socket)
{
  for(std::map<Infos, std::shared_ptr<ix::WebSocket>>::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it)
  {
    if(it->second == socket)
    {
      m_Clients.erase(it);
      break;
    }
  }
}

void WebsocketServer::try_emplace(const std::string& key, const std::shared_ptr<ix::WebSocket>& socket)
{
  std::pair<std::map<Infos, std::shared_ptr<ix::WebSocket>>::iterator, bool> ret;
  try
  {
    ret = m_Clients.try_emplace(Infos(key), socket);
  }
  catch(...)
  {
    spdlog::error("The Name \"{}\" is already taken so it cannot be connected !", ret.first->first.getName());
    Error error("The Name \"" + ret.first->first.getName() + "\" is already taken so it cannot be connected !", "", "WebSocketServer");
    for(std::map<Infos, std::shared_ptr<ix::WebSocket>>::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it)
    {
      if(it->first.getType() == "Logger") { it->second->send(error.get()); }
    }
    socket->send(error.get());
    socket->stop(200, "Exit");
  }
  if(ret.second == false)
  {
    spdlog::error("The Name \"{}\" is already taken so it cannot be connected !", ret.first->first.getName());
    Error error("The Name \"" + ret.first->first.getName() + "\" is already taken so it cannot be connected !", "", "WebSocketServer");
    for(std::map<Infos, std::shared_ptr<ix::WebSocket>>::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it)
    {
      if(it->first.getType() == "Logger") { it->second->send(error.get()); }
    }
    socket->send(error.get());
    socket->stop(200, "Exit");
  }
}

WebsocketServer::~WebsocketServer()
{
  stop();
  ix::uninitNetSystem();
}

void WebsocketServer::start()
{
  m_Server.start();
}

void WebsocketServer::stop()
{
  m_Server.stop();
}

void WebsocketServer::wait()
{
  m_Server.wait();
}

void WebsocketServer::listen()
{
  std::pair<bool, std::string> res = m_Server.listen();
  if(!res.first)
  {
    spdlog::error("{}", res.second);
    std::exit(1);
  }
}
