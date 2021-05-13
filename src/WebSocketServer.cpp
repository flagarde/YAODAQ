#include "WebSocketServer.hpp"

#include "Classes.hpp"
#include "Signals.hpp"

#include "Exception.hpp"
#include "ixwebsocket/IXNetSystem.h"
#include "Message.hpp"
#include "StatusCode.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <iostream>

#include "magic_enum.hpp"

namespace yaodaq
{

  void WebSocketServer::signalMessage()
  {
    // Skip one line
    fmt::print("\n");
    if(magic_enum::enum_integer(m_Interrupt.getSignal())>=magic_enum::enum_integer(SEVERITY::Critical)) m_Logger->critical("Signal SIG{} raised !",magic_enum::enum_name(m_Interrupt.getSignal()));
    else if (magic_enum::enum_integer(m_Interrupt.getSignal())>=magic_enum::enum_integer(SEVERITY::Error)) m_Logger->error("Signal SIG{} raised !",magic_enum::enum_name(m_Interrupt.getSignal()));
    else if (magic_enum::enum_integer(m_Interrupt.getSignal())>=magic_enum::enum_integer(SEVERITY::Warning)) m_Logger->warn("Signal SIG{} raised !",magic_enum::enum_name(m_Interrupt.getSignal()));
    else if (magic_enum::enum_integer(m_Interrupt.getSignal())>=magic_enum::enum_integer(SEVERITY::Info)) m_Logger->info("Signal SIG{} raised !",magic_enum::enum_name(m_Interrupt.getSignal()));
    else m_Logger->trace("Signal {} raised !",magic_enum::enum_name(m_Interrupt.getSignal()));
  }

  void WebSocketServer::setVerbosity(const std::string& verbosity)
  {
    if(verbosity == "off") m_Logger->set_level(spdlog::level::off);
    else if(verbosity == "trace")
      m_Logger->set_level(spdlog::level::trace);
    else if(verbosity == "info")
      m_Logger->set_level(spdlog::level::info);
    else if(verbosity == "debug")
      m_Logger->set_level(spdlog::level::debug);
    else if(verbosity == "warning")
      m_Logger->set_level(spdlog::level::warn);
    else if(verbosity == "critical")
      m_Logger->set_level(spdlog::level::critical);
  }


  int WebSocketServer::loop()
  {
    m_Logger->info("Server started on host {0} port {1}", m_Host, m_Port);
    listen();
    start();
    while(m_Interrupt.getSignal() == SIGNAL::NO )
    {
      std::this_thread::sleep_for(std::chrono::microseconds(500));
    }
    signalMessage();
    stop();
    return 0;
  }


  WebSocketServer::WebSocketServer(const std::string& name,const int& port, const std::string& host, const int& backlog,const std::size_t& maxConnections, const int& handshakeTimeoutSecs,const int& addressFamily, const std::string& type,const CLASS& _class) : m_Identifier(_class,type,name), ix::WebSocketServer(port, host, backlog, maxConnections, handshakeTimeoutSecs, addressFamily), m_Host(host), m_Port(port)
  {
    m_Interrupt.init();
    spdlog::sinks_init_list sink_list = {std::make_shared<spdlog::sinks::stdout_color_sink_mt>()};
    m_Logger                          = std::make_shared<spdlog::logger>(m_Identifier.getIdentifier(), std::begin(sink_list), std::end(sink_list));
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
        m_Logger->info("New connection ID : {}", connectionState->getId());
        m_Logger->info("Uri : {}", msg->openInfo.uri);
        m_Logger->info("Headers :");
        for(auto it: msg->openInfo.headers)
        {
          m_Logger->info("\t{} : {}", it.first, it.second);
          infos.addKey(it.first, it.second);
        }

        if(msg->openInfo.headers.find("Key") != msg->openInfo.headers.end()) { key = msg->openInfo.headers["Key"]; }
        else
        {
          key += "Browser/Browser" + std::to_string(m_BrowserNumber);
          m_Logger->info("Key : {}", key);
          ++m_BrowserNumber;
        }
        try
        {
          m_Clients.try_emplace(connectionState->getId(), key, webSocket);
        }
        catch(const Exception& exception)
        {
          m_Logger->error(exception.what());
          sendToLogger(Error(exception.what(), "ALL", "WebSocketServer").get());
          webSocket.stop(exception.getCode(),exception.getDescription());
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
        m_Logger->info("Closed connection ID : {}", connectionState->getId());
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
          m_Logger->trace("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          sendToLogger(msg->str);
        }
        else if(m_Message.getType() == "Info")
        {
          m_Logger->info("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          sendToLogger(msg->str);
        }
        else if(m_Message.getType() == "Debug")
        {
          m_Logger->debug("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          sendToLogger(msg->str);
        }
        else if(m_Message.getType() == "Warning")
        {
          m_Logger->warn("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          sendToLogger(msg->str);
        }
        else if(m_Message.getType() == "Critical")
        {
          m_Logger->critical("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          sendToLogger(msg->str);
        }
        else if(m_Message.getType() == "Error")
        {
          m_Logger->error("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          sendToLogger(msg->str);
        }
        else if(m_Message.getType() == "State")
        {
          m_Logger->warn("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          sendToAll(msg->str);
        }
        else if(m_Message.getType() == "Action")
        {
          m_Logger->warn("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          sendToAll(msg->str);
        }
        else if(m_Message.getType() == "Command")
        {
          m_Logger->warn("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          sendToAll(msg->str);
        }
        else if(m_Message.getType() == "Data")
        {
          m_Logger->warn("Sending data ({0} Mo); From : {1}; To : {2}", m_Message.getContentSize() / 1048576.0, m_Message.getFrom(), m_Message.getTo());
          sendToAll(msg->str);
        }
        else
        {
          m_Logger->info("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          sendToAll(msg->str);
        }
      }
    });
  }

  void WebSocketServer::sendToLogger(const std::string& message)
  {
    for(auto it = m_Clients.begin(); it != m_Clients.end(); ++it)
    {
      if((it->first.getType() == "Logger" || it->first.getType() == "Browser") && m_Actual != it->first.getID()) it->second.send(message);
    }
  }

  void WebSocketServer::sendToAll(const std::string& message)
  {
    for(auto it = m_Clients.begin(); it != m_Clients.end(); ++it)
    {
      if(m_Actual != it->first.getID()) it->second.send(message);
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
