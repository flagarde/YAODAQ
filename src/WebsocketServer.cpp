#include "WebsocketServer.hpp"

#include "Exception.hpp"
#include "IXNetSystem.h"
#include "Message.hpp"
#include "StatusCode.hpp"
#include "spdlog/spdlog.h"

#include <iostream>
int WebsocketServer::m_BrowserNumber = 1;

void WebsocketServer::setVerbosity(const std::string& verbosity)
{
  if(verbosity == "off") spdlog::set_level(spdlog::level::off);
  else if(verbosity == "trace")
    spdlog::set_level(spdlog::level::trace);
  else if(verbosity == "info")
    spdlog::set_level(spdlog::level::info);
  else if(verbosity == "debug")
    spdlog::set_level(spdlog::level::debug);
  else if(verbosity == "warning")
    spdlog::set_level(spdlog::level::warn);
  else if(verbosity == "critical")
    spdlog::set_level(spdlog::level::critical);
}

std::string WebsocketServer::getkey()
{
  return m_Clients3[m_Actual].getKey();
}

WebsocketServer::WebsocketServer(const int& port, const std::string& host, const int& backlog, const std::size_t& maxConnections, const int& handshakeTimeoutSecs, const int& addressFamily)
    : ix::WebSocketServer(port, host, backlog, maxConnections, handshakeTimeoutSecs, addressFamily)
{
  ix::initNetSystem();
  setOnClientMessageCallback([this](std::shared_ptr<ix::ConnectionState> connectionState,ix::WebSocket& webSocket, const ix::WebSocketMessagePtr& msg) {
    // The ConnectionInfo object contains information about the connection,
    // at this point only the client ip address and the port.
    //std::cout << "Remote ip: " << connectionInfo.remoteIp << std::endl;
    m_Actual = connectionState->getId();
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
      try_emplace(connectionState->getId(), key, webSocket);
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
      erase();
      sendToLogger(infos.get());
      spdlog::info("Closed connection ID : {}", connectionState->getId());
    }
    else if(msg->type == ix::WebSocketMessageType::Message)
    {
      Message m_Message;
      try
      {
        m_Message.parse(msg->str);
      }
      catch(const Exception& exception)
      {
        sendToLogger(Error(getkey() + " is sending not well formatted messages", "ALL", "WebServer").get());
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
      else if(m_Message.getType() == "Data")
      {
        spdlog::warn("Sending data ({0} Mo); From : {1}; To : {2}", m_Message.getContentSize() / 1048576.0, m_Message.getFrom(), m_Message.getTo());
        sendToAll(msg->str);
      }
      else
      {
        spdlog::info("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
        sendToAll(msg->str);
      }
    }
  });
}

void WebsocketServer::sendToLogger(const std::string& message)
{
  for(std::map<std::string, ix::WebSocket&>::iterator it = m_Clients2.begin(); it != m_Clients2.end(); ++it)
  {
    if((m_Clients3[it->first].getType() == "Logger" || m_Clients3[it->first].getType() == "Browser") && m_Actual != it->first) it->second.send(message);
  }
}

void WebsocketServer::erase()
{
  m_Clients2.erase(m_Actual);
  m_Clients3.erase(m_Actual);
}

void WebsocketServer::try_emplace(const std::string& id, const std::string& key, ix::WebSocket& socket)
{
  for(std::map<std::string, Infos>::iterator it = m_Clients3.begin(); it != m_Clients3.end(); ++it)
  {
    if(it->second.getKey() == key)
    {
      spdlog::error("The Name \"{}\" is already taken so it cannot be connected !", it->second.getName());
      sendToLogger(Error("The Name \"" + it->second.getName() + "\" is already taken so it cannot be connected !", "ALL", "WebSocketServer").get());
      socket.stop(static_cast<uint16_t>(StatusCode::ALREADY_PRESENT), "The Name \"" + it->second.getName() + "\" is already taken so it cannot be connected !");
      return;
    }
  }
  m_Clients2.try_emplace(id, socket);
  m_Clients3.try_emplace(id, Infos(key));
}

void WebsocketServer::sendToAll(const std::string& message)
{
  for(std::map<std::string, ix::WebSocket&>::iterator it = m_Clients2.begin(); it != m_Clients2.end(); ++it)
  {
    if(m_Actual != it->first) it->second.send(message);
  }
}

WebsocketServer::~WebsocketServer()
{
  stop();
  ix::uninitNetSystem();
  //std::exit(3);
}

void WebsocketServer::listen()
{
  std::pair<bool, std::string> res = ix::WebSocketServer::listen();
  if(!res.first) { Exception(StatusCode::FAILURE, res.second); }
}
