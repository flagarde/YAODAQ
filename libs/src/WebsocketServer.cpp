#include "WebsocketServer.hpp"

#include "Exception.hpp"
#include "IXNetSystem.h"
#include "Message.hpp"
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
  throw Exception(STATUS_CODE_NOT_FOUND, "Client not found !");
}

WebsocketServer::WebsocketServer(const int& port, const std::string& host, const int& backlog, const std::size_t& maxConnections,
                                 const int& handshakeTimeoutSecs)
    : m_Server(port, host, backlog, maxConnections, handshakeTimeoutSecs)
{
  ix::initNetSystem();
  m_Server.setOnConnectionCallback([this](std::shared_ptr<ix::WebSocket> webSocket, std::shared_ptr<ix::ConnectionState> connectionState) {
    webSocket->setOnMessageCallback([webSocket, connectionState, this](const ix::WebSocketMessagePtr& msg) {
      m_Actual = webSocket;
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
          key += "Browser/Browser" + std::to_string(m_BrowserNumber);
          spdlog::info("Key : {}", key);
          ++m_BrowserNumber;
        }
        try_emplace(key, webSocket);
        webSocket->send(State(m_State, "ALL", "WebServer").get());
        sendToLogger("New connection ID : " + connectionState->getId() + " Key : " + key + " Host : " + msg->openInfo.headers["Host"]);
      }
      else if(msg->type == ix::WebSocketMessageType::Close)
      {
        erase(webSocket);
        //webSocket->close();
        spdlog::info("Closed connection ID : {}", connectionState->getId());
        sendToLogger("Closed connection ID : " + connectionState->getId());
      }
      else if(msg->type == ix::WebSocketMessageType::Message)
      {
        Message message;
        try
        {
          message.parse(msg->str);
          message.setFrom(getkey(webSocket));
        }
        catch(const Exception& exception)
        {
        }
        if(message.getType() == "Trace")
        {
          spdlog::trace("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo());
          sendToLogger(msg->str);
        }
        else if(message.getType() == "Info")
        {
          spdlog::info("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo());
          sendToLogger(msg->str);
        }
        else if(message.getType() == "Debug")
        {
          spdlog::debug("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo());
          sendToLogger(msg->str);
        }
        else if(message.getType() == "Warning")
        {
          spdlog::warn("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo());
          sendToLogger(msg->str);
        }
        else if(message.getType() == "Critical")
        {
          spdlog::critical("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo());
          sendToLogger(msg->str);
        }
        else if(message.getType() == "Error")
        {
          spdlog::error("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo());
          sendToLogger(msg->str);
        }
        else if(message.getType() == "State")
        {
          spdlog::warn("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo());
          auto state = magic_enum::enum_cast<States>(message.getContent());
          if(state.has_value()) { m_State = state.value(); }
          sendToAll(msg->str);
        }
        else if(message.getType() == "Action")
        {
          spdlog::warn("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo());
          sendToAll(msg->str);
        }
        else if(message.getType() == "Command")
        {
          spdlog::warn("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo());
          sendToAll(msg->str);
        }
        else
        {
          spdlog::info("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo());
          sendToAll(msg->str);
        }
      }
    });
  });
}

void WebsocketServer::sendToLogger(const std::string& message)
{
  //std::lock_guard<std::mutex> guard(m_Mutex);
  for(std::map<Infos, std::shared_ptr<ix::WebSocket>>::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it)
  {
    if(it->first.getType() == "Logger" && m_Actual != it->second) it->second->send(message);
  }
}

void WebsocketServer::erase(const std::shared_ptr<ix::WebSocket>& socket)
{
  for(std::map<Infos, std::shared_ptr<ix::WebSocket>>::iterator it = m_Clients.begin(); it != m_Clients.end();)
  {
    if(it->second == socket) { m_Clients.erase(it->first); }
    else
      ++it;
  }
}

void WebsocketServer::try_emplace(const std::string& key, const std::shared_ptr<ix::WebSocket>& socket)
{
  std::map<Infos, std::shared_ptr<ix::WebSocket>>::iterator ret = m_Clients.find(Infos(key));
  if(ret != m_Clients.end())
  {
    spdlog::error("The Name \"{}\" is already taken so it cannot be connected !", ret->first.getName());
    Error error("The Name \"" + ret->first.getName() + "\" is already taken so it cannot be connected !", "", "WebSocketServer");
    sendToLogger(error.get());
    socket->stop(ix::WebSocketCloseConstants::kInternalErrorCode,
                 "The Name \"" + ret->first.getName() + "\" is already taken so it cannot be connected !");
  }
  else
  {
    std::cout << "EMplace " << key << std::endl;
    m_Clients.try_emplace(Infos(key), socket);
  }
}

void WebsocketServer::sendToAll(const std::string& message)
{
  for(std::map<Infos, std::shared_ptr<ix::WebSocket>>::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it)
  {
    std::cout << "send to " << it->first.getName() << std::endl;
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
  if(!res.first) { Exception(STATUS_CODE_FAILURE, res.second); }
}
