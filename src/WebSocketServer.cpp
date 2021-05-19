#include "WebSocketServer.hpp"


#include "ixwebsocket/IXNetSystem.h"

#include "spdlog/logger.h"

#include "Classes.hpp"

#include "Exception.hpp"

#include "Message.hpp"
#include "StatusCode.hpp"

#include "magic_enum.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <iostream>
namespace yaodaq
{


  int WebSocketServer::loop()
  {
    m_LoggerHandler.addSink(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    logger()->info("Server started on host {0} port {1}", m_Host, m_Port);
    listen();
    start();
    onRaisingSignal();
    stop();
    return 0;
  }


  WebSocketServer::WebSocketServer(const std::string& name,const int& port, const std::string& host, const int& backlog,const std::size_t& maxConnections, const int& handshakeTimeoutSecs,const int& addressFamily, const std::string& type,const CLASS& _class) : m_Identifier(_class,type,name), ix::WebSocketServer(port, host, backlog, maxConnections, handshakeTimeoutSecs, addressFamily), m_Host(host), m_Port(port)
  {
    m_Interrupt.init();
    m_LoggerHandler.setName(m_Identifier.getIdentifier());
    ix::initNetSystem();
   // m_LoggerHandler.addDefaultSink();
    //spdlog::sinks_init_list sink_list = {std::make_shared<spdlog::sinks::stdout_color_sink_mt>()};
    //m_Logger                          = std::make_shared<spdlog::logger>(m_Identifier.getIdentifier(), std::begin(sink_list), std::end(sink_list));

    setOnClientMessageCallback(/*getMessageCallback());*/



      [this](std::shared_ptr<ix::ConnectionState> connectionState,ix::WebSocket& webSocket, const ix::WebSocketMessagePtr& msg) {
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
        logger()->info("New connection ID : {}", connectionState->getId());
        logger()->info("Uri : {}", msg->openInfo.uri);
        logger()->info("Headers :");
        for(auto it: msg->openInfo.headers)
        {
          logger()->info("\t{} : {}", it.first, it.second);
          infos.addKey(it.first, it.second);
        }

        if(msg->openInfo.headers.find("Key") != msg->openInfo.headers.end()) { key = msg->openInfo.headers["Key"]; }
        else
        {
          key += "Browser/Browser" + std::to_string(m_BrowserNumber);
          logger()->info("Key : {}", key);
          ++m_BrowserNumber;
        }
        try
        {
          m_Clients.try_emplace(connectionState->getId(), key, webSocket);
        }
        catch(const Exception& exception)
        {
          logger()->error(exception.what());
          sendToLogger(Error(exception.what(), "ALL", "WebSocketServer").get());
          webSocket.stop(exception.code(),exception.description());
        }
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
        m_Clients.erase(connectionState->getId());
        sendToLogger(infos.get());
        logger()->info("Closed connection ID : {}", connectionState->getId());
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
          sendToLogger(Error(m_Clients.getInfos(connectionState->getId()).getKey() + " is sending not well formatted messages", "ALL", "WebServer").get());
        }
        if(m_Message.getType() == "Trace")
        {
          logger()->trace("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          sendToLogger(msg->str);
        }
        else if(m_Message.getType() == "Info")
        {
          logger()->info("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          sendToLogger(msg->str);
        }
        else if(m_Message.getType() == "Debug")
        {
          logger()->debug("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          sendToLogger(msg->str);
        }
        else if(m_Message.getType() == "Warning")
        {
          logger()->warn("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          sendToLogger(msg->str);
        }
        else if(m_Message.getType() == "Critical")
        {
          logger()->critical("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          sendToLogger(msg->str);
        }
        else if(m_Message.getType() == "Error")
        {
          logger()->error("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          sendToLogger(msg->str);
        }
        else if(m_Message.getType() == "State")
        {
          logger()->warn("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          sendToAll(msg->str);
        }
        else if(m_Message.getType() == "Action")
        {
          logger()->warn("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          sendToAll(msg->str);
        }
        else if(m_Message.getType() == "Command")
        {
          logger()->warn("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          sendToAll(msg->str);
        }
        else if(m_Message.getType() == "Data")
        {
          logger()->warn("Sending data ({0} Mo); From : {1}; To : {2}", m_Message.getContentSize() / 1048576.0, m_Message.getFrom(), m_Message.getTo());
          sendToAll(msg->str);
        }
        else
        {
          logger()->info("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          sendToAll(msg->str);
        }
      }
    });

  }

  void WebSocketServer::sendToLogger(const std::string& message)
  {
    for(auto it = m_Clients.begin(); it != m_Clients.end(); ++it)
    {
      if((it->first.getType() == "Logger" || it->first.getType() == "Browser") && getClient() != it->first.getID()) it->second.send(message);
    }
  }

  void WebSocketServer::sendToAll(const std::string& message)
  {
    for(auto it = m_Clients.begin(); it != m_Clients.end(); ++it)
    {
      if(getClient() != it->first.getID()) it->second.send(message);
    }
  }

  WebSocketServer::~WebSocketServer()
  {
    stop();
    ix::uninitNetSystem();
  }

  void WebSocketServer::listen()
  {
    std::pair<bool, std::string> res = ix::WebSocketServer::listen();
    if(!res.first) { Exception(StatusCode::FAILURE, res.second); }
  }

};
