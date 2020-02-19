#include "Logger.hpp" 
#include "spdlog.h"
#include "Error.hpp"

Logger::Logger(const std::string& name,const std::string& type):m_Name(name),m_Type(type)
{
  m_WebsocketClient.setExtraHeader("Key","///"+m_Type+"/"+m_Name);
  m_WebsocketClient.setOnMessageCallback(m_CallBack);
  m_WebsocketClient.start();
}

Logger::~Logger()
{
}

void Logger::OnOpen(const ix::WebSocketMessagePtr& msg)
{
  spdlog::info("Connected !");
  spdlog::info("Handshake Headers :");
  for (auto it : msg->openInfo.headers)
  {
    spdlog::info("\t{0}:{1}",it.first,it.second);
  }
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
  spdlog::info("Disconnected !");
  spdlog::info("{}",msg->closeInfo.code);
  spdlog::info("{}",msg->closeInfo.reason); 
  if(msg->closeInfo.code==1002) throw Error(msg->closeInfo.code,msg->closeInfo.reason);
}

void Logger::OnPong(const ix::WebSocketMessagePtr& msg)
{
  spdlog::info("Pong data : {}",msg->str);
}

void Logger::OnPing(const ix::WebSocketMessagePtr& msg)
{
  spdlog::info("Ping data : {}",msg->str);
}

void Logger::OnMessage(const ix::WebSocketMessagePtr& msg)
{
  spdlog::info("{}",msg->str);
}

void Logger::OnError(const ix::WebSocketMessagePtr& msg)
{
  spdlog::error("Error : {}",msg->errorInfo.reason);
  spdlog::error("#retries : {}",msg->errorInfo.retries);
  spdlog::error("Wait time(ms) : {}",msg->errorInfo.wait_time);
  spdlog::error("HTTP Status : {}",msg->errorInfo.http_status);
}
