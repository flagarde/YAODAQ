#include "Controller.hpp"

#include "StatusCode.hpp"
#include "magic_enum.hpp"
#include "spdlog/sinks/ansicolor_sink.h"
#include "spdlog/sinks/ostream_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include "toml.hpp"
std::string Controller::getName()
{
  return m_Name;
}
std::string Controller::getType()
{
  return m_Type;
}

ix::WebSocketSendInfo Controller::sendBinary(Message& message)
{
  message.setFrom(m_Type + "/" + m_Name);
  return m_WebsocketClient.sendBinary(message.get());
}

ix::WebSocketSendInfo Controller::sendText(Message& message)
{
  message.setFrom(m_Type + "/" + m_Name);
  return m_WebsocketClient.sendText(message.get());
}

Controller::Controller(const std::string& name, const std::string& type): m_Type(type), m_Name(name)
{
  spdlog::sinks_init_list sink_list = {std::make_shared<spdlog::sinks::stdout_color_sink_mt>()};
  m_Logger                          = std::make_shared<spdlog::logger>(m_Type + "/" + m_Name, std::begin(sink_list), std::end(sink_list));
  // Mimic json to parse the message and the level to change it on Loggers;
  m_WebsocketClient.setHeaderKey("Key", "///" + m_Type + "/" + m_Name);
  m_WebsocketClient.setOnMessageCallback(m_CallBack);
  m_WebsocketClient.start();
}

void Controller::sendAction(const std::string& action)
{
  auto ac = magic_enum::enum_cast<Actions>(action);
  if(ac.has_value())
  {
    Action a(ac.value());
    sendBinary(a);
  }
  else
    throw Exception(StatusCode::INVALID_PARAMETER, "{} is not a valid Action", action);
}

void Controller::sendCommand(const std::string& command)
{
  Command m_command(command);
  sendBinary(m_command);
}

void Controller::DoOnMessage(const ix::WebSocketMessagePtr& msg)
{
  Message message;
  message.parse(msg->str);
  if(message.getType() == "State") { spdlog::warn("{}", msg->str); }
}

void Controller::stop()
{
  m_WebsocketClient.stop();
}

void Controller::OnOpen(const ix::WebSocketMessagePtr& msg)
{
  m_Logger->info("Handshake Headers :");
  for(auto it: msg->openInfo.headers) { m_Logger->info("\t{0}:{1}", it.first, it.second); }
  m_Logger->info("");
}

void Controller::OnClose(const ix::WebSocketMessagePtr& msg)
{
  // The server can send an explicit code and reason for closing.
  // This data can be accessed through the closeInfo object.
  if(msg->closeInfo.code == static_cast<int16_t>(StatusCode::ALREADY_PRESENT))
  {
    m_WebsocketClient.disableAutomaticReconnection();
    throw Exception(StatusCode::ALREADY_PRESENT, msg->closeInfo.reason);
  }
  spdlog::info("{}", msg->closeInfo.code);
  spdlog::info("{}", msg->closeInfo.reason);
}

void Controller::OnPong(const ix::WebSocketMessagePtr& msg)
{
  m_Logger->info("Pong data : {}", msg->str);
}

void Controller::OnPing(const ix::WebSocketMessagePtr& msg)
{
  m_Logger->info("Ping data : {}", msg->str);
}

void Controller::OnMessage(const ix::WebSocketMessagePtr& msg)
{
  m_Logger->info("{}", msg->str);
}

void Controller::OnError(const ix::WebSocketMessagePtr& msg)
{
  m_Logger->error("Error : {}", msg->errorInfo.reason);
  m_Logger->error("#retries : {}", msg->errorInfo.retries);
  m_Logger->error("Wait time(ms) : {}", msg->errorInfo.wait_time);
  m_Logger->error("HTTP Status : {}", msg->errorInfo.http_status);
}
