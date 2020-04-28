#include "Logger.hpp"

#include "Exception.hpp"
#include "Interrupt.hpp"
#include "Message.hpp"
#include "spdlog.h"

#include <iostream>

Json::StreamWriterBuilder Logger::m_StreamWriterBuilder = Json::StreamWriterBuilder();

Json::CharReaderBuilder Logger::m_CharReaderBuilder = Json::CharReaderBuilder();

Logger::Logger(const std::string& name, const std::string& type): m_Name(name), m_Type(type)
{
  m_Writer.reset(m_StreamWriterBuilder.newStreamWriter());
  m_Reader.reset(m_CharReaderBuilder.newCharReader());
  m_WebsocketClient.setExtraHeader("Key", "///" + m_Type + "/" + m_Name);
  m_WebsocketClient.setOnMessageCallback(m_CallBack);
  m_WebsocketClient.start();
}

Logger::~Logger()
{
  m_WebsocketClient.stop();
}

void Logger::OnOpen(const ix::WebSocketMessagePtr& msg)
{
  spdlog::info("Handshake Headers :");
  for(auto it: msg->openInfo.headers) { spdlog::info("\t{0}:{1}", it.first, it.second); }
  spdlog::info("");
}

void Logger::stop()
{
  m_WebsocketClient.stop();
}

ix::ReadyState Logger::getReadyState()
{
  return m_WebsocketClient.getReadyState();
}

void Logger::OnClose(const ix::WebSocketMessagePtr& msg)
{
  // The server can send an explicit code and reason for closing.
  // This data can be accessed through the closeInfo object.
  spdlog::info("{}", msg->closeInfo.code);
  spdlog::info("{}", msg->closeInfo.reason);
}

void Logger::OnPong(const ix::WebSocketMessagePtr& msg)
{
  spdlog::info("Pong data : {}", msg->str);
}

void Logger::OnPing(const ix::WebSocketMessagePtr& msg)
{
  spdlog::info("Ping data : {}", msg->str);
}

void Logger::OnMessage(const ix::WebSocketMessagePtr& msg)
{
  Message message;
  try
  {
    message.parse(msg->str);
  }
  catch(...)
  {
    spdlog::info("{}", msg->str);
  }
  if(message.getType() == "Trace") { spdlog::trace("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo()); }
  else if(message.getType() == "Info")
  {
    if(!message.isEmpty()) spdlog::info("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo());
  }
  else if(message.getType() == "Debug")
  {
    spdlog::debug("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo());
  }
  else if(message.getType() == "Warning")
  {
    spdlog::warn("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo());
  }
  else if(message.getType() == "Critical")
  {
    spdlog::critical("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo());
  }
  else if(message.getType() == "Error")
  {
    spdlog::error("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo());
  }
  else if(message.getType() == "State")
  {
    spdlog::warn("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo());
  }
  else if(message.getType() == "Action")
  {
    spdlog::warn("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo());
  }
  else if(message.getType() == "Command")
  {
    spdlog::warn("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo());
  }
  else
  {
    spdlog::info("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo());
  }
}

void Logger::OnError(const ix::WebSocketMessagePtr& msg)
{
  spdlog::error("Error : {}", msg->errorInfo.reason);
  spdlog::error("#retries : {}", msg->errorInfo.retries);
  spdlog::error("Wait time(ms) : {}", msg->errorInfo.wait_time);
  spdlog::error("HTTP Status : {}", msg->errorInfo.http_status);
}
