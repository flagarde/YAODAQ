#include "Controller.hpp"

#include "LoggerSink.hpp"
#include "sinks/ansicolor_sink.h"
#include "sinks/ostream_sink.h"
#include "sinks/stdout_color_sinks.h"
#include "spdlog.h"
#include "toml.hpp"

std::string Controller::getName() { return m_Name; }
std::string Controller::getType() { return m_Type; }

ix::WebSocketSendInfo Controller::sendBinary(Message &message) {
  message.setFrom(m_Type + "/" + m_Name);
  return m_WebsocketClient.sendBinary(message.get());
}

ix::WebSocketSendInfo Controller::sendText(Message &message) {
  message.setFrom(m_Type + "/" + m_Name);
  return m_WebsocketClient.sendText(message.get());
}

Controller::Controller(const std::string &name, const std::string &type)
    : m_Type(type), m_Name(name) {
  spdlog::sinks_init_list sink_list = {
      std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
      std::make_shared<WebSocketLoguer_mt>(m_WebsocketClient,
                                           m_Type + "/" + m_Name)};
  m_Logger = std::make_shared<spdlog::logger>(
      m_Type + "/" + m_Name, std::begin(sink_list), std::end(sink_list));
  // Mimic json to parse the message and the level to change it on Loggers;
  std::string pattern = "{\"Message\" : \"%v\", \"Level\" : \"%l\"}";
  m_Logger->sinks()[1]->set_pattern(pattern);
  m_WebsocketClient.setExtraHeader("Key", "///" + m_Type + "/" + m_Name);
  m_WebsocketClient.setOnMessageCallback(m_CallBack);
  m_WebsocketClient.start();
}

Controller::~Controller() {}

void Controller::sendStatus(const std::string &stat) {
  Status status;
  status.setContent(stat);
  sendText(status);
}

void Controller::DoOnMessage(const ix::WebSocketMessagePtr &msg) {
  Message message;
  message.parse(msg->str);
  if (message.getType() == "Status") {
  }
}

void Controller::OnOpen(const ix::WebSocketMessagePtr &msg) {
  m_Logger->info("Connected !");
  m_Logger->info("Handshake Headers :");
  for (auto it : msg->openInfo.headers) {
    m_Logger->info("\t{0}:{1}", it.first, it.second);
  }
  m_Logger->info("");
}

void Controller::OnClose(const ix::WebSocketMessagePtr &msg) {
  // The server can send an explicit code and reason for closing.
  // This data can be accessed through the closeInfo object.
  m_Logger->info("Disconnected !");
  m_Logger->info("{}", msg->closeInfo.code);
  m_Logger->info("{}", msg->closeInfo.reason);
  if (msg->closeInfo.code == 1002)
    throw Error(msg->closeInfo.code, msg->closeInfo.reason);
}

void Controller::OnPong(const ix::WebSocketMessagePtr &msg) {
  m_Logger->info("Pong data : {}", msg->str);
}

void Controller::OnPing(const ix::WebSocketMessagePtr &msg) {
  m_Logger->info("Ping data : {}", msg->str);
}

void Controller::OnMessage(const ix::WebSocketMessagePtr &msg) {
  m_Logger->info("{}", msg->str);
}

void Controller::OnError(const ix::WebSocketMessagePtr &msg) {
  m_Logger->error("Error : {}", msg->errorInfo.reason);
  m_Logger->error("#retries : {}", msg->errorInfo.retries);
  m_Logger->error("Wait time(ms) : {}", msg->errorInfo.wait_time);
  m_Logger->error("HTTP Status : {}", msg->errorInfo.http_status);
}
