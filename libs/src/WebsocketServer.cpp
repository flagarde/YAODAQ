#include "WebsocketServer.hpp"

#include "Exception.hpp"
#include "IXNetSystem.h"
#include "StatusCode.hpp"
#include "json.h"
#include "magic_enum.hpp"
#include "spdlog.h"

#include <algorithm>
#include <iostream>

int WebsocketServer::m_BrowserNumber = 1;

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

std::string WebsocketServer::getkey(const std::shared_ptr<ix::WebSocket>& websocket)
{
  for(std::map<Infos, std::shared_ptr<ix::WebSocket>>::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it)
  {
    if(it->second == websocket) return it->first.getKey();
  }
  throw Exception(StatusCode::NOT_FOUND, "Client not found !");
}

WebsocketServer::WebsocketServer(const int& port, const std::string& host, const int& backlog, const std::size_t& maxConnections, const int& handshakeTimeoutSecs)
    : m_Server(port, host, backlog, maxConnections, handshakeTimeoutSecs)
{
  ix::initNetSystem();
  m_Server.setOnConnectionCallback([this](std::shared_ptr<ix::WebSocket> webSocket, std::shared_ptr<ix::ConnectionState> connectionState) {
    webSocket->setOnMessageCallback([webSocket, connectionState, this](const ix::WebSocketMessagePtr& msg) {
      m_Actual = webSocket;
      if(msg->type == ix::WebSocketMessageType::Open)
      {
        std::string key = "///";
        Info        infos;
        infos.setFrom("WebSocketServer");
        infos.setTo("ALL");
        // Print in webserver console
        spdlog::info("New connection ID : {}", connectionState->getId());
        spdlog::info("Uri : {}", msg->openInfo.uri);
        spdlog::info("Headers :");
        for(auto it: msg->openInfo.headers)
        {
          spdlog::info("\t{} : {}", it.first, it.second);
          infos.addKey(it.first, it.second);
        }

        if(msg->openInfo.headers.find("Key") != msg->openInfo.headers.end()) { key = msg->openInfo.headers["Key"]; }
        else
        {
          key += "Browser/Browser" + std::to_string(m_BrowserNumber);
          spdlog::info("Key : {}", key);
          ++m_BrowserNumber;
        }
        try_emplace(key, webSocket);
        webSocket->send(State(m_State, "ALL", "WebServer").get());
        infos.addKey("ID", connectionState->getId());
        infos.addKey("Key", key);
        infos.addKey("Host", msg->openInfo.headers["Host"]);
        infos.addKey("Value", "CONNECTED");
        sendToLogger(infos.get());
      }
      else if(msg->type == ix::WebSocketMessageType::Close)
      {
        Info infos;
        infos.setFrom("WebSocketServer");
        infos.setTo("ALL");
        infos.addKey("ID", connectionState->getId());
        infos.addKey("Value", "DISCONNECTED");
        erase(webSocket);
        sendToLogger(infos.get());
        spdlog::info("Closed connection ID : {}", connectionState->getId());
      }
      else if(msg->type == ix::WebSocketMessageType::Message)
      {
        try
        {
          m_Message.parse(msg->str);
          m_Message.setFrom(getkey(webSocket));
        }
        catch(const Exception& exception)
        {
          sendToLogger(Error(getkey(webSocket) + " is sending not well formatted messages", "ALL", "WebServer").get());
        }
        if(m_Message.getType() == "Trace")
        {
          spdlog::trace("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          sendToLogger(msg->str);
        }
        else if(m_Message.getType() == "Info")
        {
          spdlog::info("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          sendToLogger(msg->str);
        }
        else if(m_Message.getType() == "Debug")
        {
          spdlog::debug("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          sendToLogger(msg->str);
        }
        else if(m_Message.getType() == "Warning")
        {
          spdlog::warn("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          sendToLogger(msg->str);
        }
        else if(m_Message.getType() == "Critical")
        {
          spdlog::critical("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          sendToLogger(msg->str);
        }
        else if(m_Message.getType() == "Error")
        {
          spdlog::error("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          sendToLogger(msg->str);
        }
        else if(m_Message.getType() == "State")
        {
          spdlog::warn("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          auto state = magic_enum::enum_cast<States>(m_Message.getContent());
          if(state.has_value()) { m_State = state.value(); }
          sendToAll(msg->str);
        }
        else if(m_Message.getType() == "Action")
        {
          spdlog::warn("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          sendToAll(msg->str);
        }
        else if(m_Message.getType() == "Command")
        {
          spdlog::warn("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          sendToAll(msg->str);
        }
        else
        {
          spdlog::info("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          sendToAll(msg->str);
        }
      }
    });
  });
}

void WebsocketServer::sendToLogger(const std::string& message)
{
  for(std::map<Infos, std::shared_ptr<ix::WebSocket>>::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it)
  {
    if(it->first.getType() == "Logger" && m_Actual != it->second) it->second->send(message);
  }
}

void WebsocketServer::erase(const std::shared_ptr<ix::WebSocket>& socket)
{
  for(std::map<Infos, std::shared_ptr<ix::WebSocket>>::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it)
  {
    if(it->second == socket)
    {
      m_Clients.erase(it->first);
      break;
    }
  }
}

void WebsocketServer::try_emplace(const std::string& key, const std::shared_ptr<ix::WebSocket>& socket)
{
  std::map<Infos, std::shared_ptr<ix::WebSocket>>::iterator ret = m_Clients.find(Infos(key));
  if(ret != m_Clients.end())
  {
    m_Clients.try_emplace(Infos(key + "ERASE"), socket);
    spdlog::error("The Name \"{}\" is already taken so it cannot be connected !", ret->first.getName());
    sendToLogger(Error("The Name \"" + ret->first.getName() + "\" is already taken so it cannot be connected !", "ALL", "WebSocketServer").get());
    socket->stop(static_cast<uint16_t>(StatusCode::ALREADY_PRESENT), "The Name \"" + ret->first.getName() + "\" is already taken so it cannot be connected !");
  }
  else
  {
    m_Clients.try_emplace(Infos(key), socket);
  }
}

void WebsocketServer::sendToAll(const std::string& message)
{
  for(std::map<Infos, std::shared_ptr<ix::WebSocket>>::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it)
  {
    if(m_Actual != it->second) it->second->send(message);
  }
}

WebsocketServer::~WebsocketServer()
{
  m_Server.stop();
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
  if(!res.first) { Exception(StatusCode::FAILURE, res.second); }
}
