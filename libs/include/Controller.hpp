#pragma once

#include "Exception.hpp"
#include "Message.hpp"
#include "WebsocketClient.hpp"
#include "spdlog.h"

#include <iostream>
#include <string>

class Controller
{
public:
  Controller(const std::string& name = "", const std::string& type = "Controller");
  virtual ~Controller();
  std::string           getName();
  std::string           getType();
  void                  stop();
  virtual void          OnOpen(const ix::WebSocketMessagePtr& msg);
  virtual void          OnClose(const ix::WebSocketMessagePtr& msg);
  virtual void          OnPong(const ix::WebSocketMessagePtr& msg);
  virtual void          OnPing(const ix::WebSocketMessagePtr& msg);
  virtual void          OnMessage(const ix::WebSocketMessagePtr& msg);
  virtual void          OnError(const ix::WebSocketMessagePtr& msg);
  ix::WebSocketSendInfo sendBinary(Message& message);
  ix::WebSocketSendInfo sendText(Message& message);
  void                  sendAction(const std::string& action);
  void                  sendCommand(const std::string& command);

protected:
  std::string                     m_Name{"Unknown"};
  std::string                     m_Type{"Unknown"};
  std::shared_ptr<spdlog::logger> m_Logger{nullptr};

private:
  void                                                DoOnMessage(const ix::WebSocketMessagePtr& msg);
  WebsocketClient                                     m_WebsocketClient;
  std::function<void(const ix::WebSocketMessagePtr&)> m_CallBack{[this](const ix::WebSocketMessagePtr& msg) {
    if(msg->type == ix::WebSocketMessageType::Message) { this->DoOnMessage(msg); }
    else if(msg->type == ix::WebSocketMessageType::Open)
    {
      this->OnOpen(msg);
    }
    else if(msg->type == ix::WebSocketMessageType::Close)
    {
      this->OnClose(msg);
    }
    else if(msg->type == ix::WebSocketMessageType::Error)
    {
      this->OnError(msg);
    }
    else if(msg->type == ix::WebSocketMessageType::Ping)
    {
      this->OnPing(msg);
    }
    else if(msg->type == ix::WebSocketMessageType::Pong)
    {
      this->OnPong(msg);
    }
  }};
};
