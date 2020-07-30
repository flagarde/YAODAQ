#pragma once
#include "WebsocketClient.hpp"

class Logger
{
public:
  Logger(const std::string& name = "", const std::string& type = "Logger");
  virtual ~Logger() = default;
  void           stop();
  virtual void   OnOpen(const ix::WebSocketMessagePtr& msg);
  virtual void   OnClose(const ix::WebSocketMessagePtr& msg);
  virtual void   OnPong(const ix::WebSocketMessagePtr& msg);
  virtual void   OnPing(const ix::WebSocketMessagePtr& msg);
  virtual void   OnMessage(const ix::WebSocketMessagePtr& msg);
  virtual void   OnError(const ix::WebSocketMessagePtr& msg);
  ix::ReadyState getReadyState();

protected:
  std::string m_Name{"Unknown"};
  std::string m_Type{"Unknown"};

private:
  WebsocketClient                                     m_WebsocketClient;
  std::function<void(const ix::WebSocketMessagePtr&)> m_CallBack{[this](const ix::WebSocketMessagePtr& msg) {
    if(msg->type == ix::WebSocketMessageType::Message) { this->OnMessage(msg); }
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
