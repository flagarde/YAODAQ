#include "Logger.hpp"

#include "Error.hpp"
#include "Message.hpp"
#include "spdlog.h"

Json::StreamWriterBuilder Logger::m_StreamWriterBuilder =
    Json::StreamWriterBuilder();

Json::CharReaderBuilder Logger::m_CharReaderBuilder = Json::CharReaderBuilder();

Logger::Logger(const std::string& name, const std::string& type)
    : m_Name(name), m_Type(type)
{
  m_Writer.reset(m_StreamWriterBuilder.newStreamWriter());
  m_Reader.reset(m_CharReaderBuilder.newCharReader());
  m_WebsocketClient.setExtraHeader("Key", "///" + m_Type + "/" + m_Name);
  m_WebsocketClient.setOnMessageCallback(m_CallBack);
  m_WebsocketClient.start();
}

Logger::~Logger() {}

void Logger::OnOpen(const ix::WebSocketMessagePtr& msg)
{
  spdlog::info("Handshake Headers :");
  for(auto it: msg->openInfo.headers)
  { spdlog::info("\t{0}:{1}", it.first, it.second); }
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
  if(msg->closeInfo.code == 1002)
    throw Error(msg->closeInfo.code, msg->closeInfo.reason);
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
    return;
  }
  // If it's Log only send to Loggers :)

  if(message.getType() == "Log")
  {
    bool ok = m_Reader->parse(
        &(message.getContent()[0]),
        &message.getContent()[message.getContent().size()], &m_Value, &m_Errs);
    if(!ok)
    {
      spdlog::error("Problem parsing Log message to JSON {}",
                    message.getContent());
    }
    else
    {
      std::string Level   = m_Value["Level"].asString();
      std::string content = m_Value["Message"].asString();
      if(Level == "trace")
        spdlog::trace("[" + message.getFrom() + "] " + content);
      else if(Level == "debug")
        spdlog::debug("[" + message.getFrom() + "] " + content);
      else if(Level == "info")
        spdlog::info("[" + message.getFrom() + "] " + content);
      else if(Level == "warning")
        spdlog::warn("[" + message.getFrom() + "] " + content);
      else if(Level == "error")
        spdlog::error("[" + message.getFrom() + "] " + content);
      else if(Level == "critical")
        spdlog::critical("[" + message.getFrom() + "] " + content);
    }
  }
  else
    spdlog::info("{}", msg->str);
}

void Logger::OnError(const ix::WebSocketMessagePtr& msg)
{
  spdlog::error("Error : {}", msg->errorInfo.reason);
  spdlog::error("#retries : {}", msg->errorInfo.retries);
  spdlog::error("Wait time(ms) : {}", msg->errorInfo.wait_time);
  spdlog::error("HTTP Status : {}", msg->errorInfo.http_status);
}
