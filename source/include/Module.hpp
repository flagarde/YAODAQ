#ifndef MODULE_HPP
#define MODULE_HPP

#include <iostream>
#include <sstream>
#include <string>
#include "Message.hpp"
#include "Info.hpp"
#include "Status.hpp"
#include "WebsocketClient.hpp"

class Module 
{
public :
  Module( const std::string& type="Unknown",const std::string& name="Unknown");
  virtual ~Module();
  virtual void Initialize();
  virtual void Connect();
  virtual void Configure();
  virtual void Start();
  virtual void Pause();
  virtual void Stop();
  virtual void Clear();
  virtual void Disconnect();
  virtual void Release();
  virtual void Quit();
  std::string getName();
  std::string getType();
  virtual void OnOpen(const ix::WebSocketMessagePtr& msg);
  virtual void OnClose(const ix::WebSocketMessagePtr& msg);
  virtual void OnPong(const ix::WebSocketMessagePtr& msg);
  virtual void OnPing(const ix::WebSocketMessagePtr& msg);
  virtual void OnMessage(const ix::WebSocketMessagePtr& msg);
  virtual void OnError(const ix::WebSocketMessagePtr& msg);
  
  ix::WebSocketSendInfo sendBinary(Message& message);
  ix::WebSocketSendInfo sendText(Message& message);
  
private:
  void DoOnStatus(Message& message);
  void DoOnMessage(const ix::WebSocketMessagePtr& msg);
  WebsocketClient m_WebsocketClient;
  std::string m_Name{"Unknown"};
  std::string m_Type{"Unknown"};
  std::function<void(const ix::WebSocketMessagePtr&)> m_CallBack
  {
    [this](const ix::WebSocketMessagePtr& msg)
    {
      if (msg->type == ix::WebSocketMessageType::Message)
      {
        this->DoOnMessage(msg);
      }
      else if (msg->type == ix::WebSocketMessageType::Open)
      {
        this->OnOpen(msg);
      }
      else if (msg->type == ix::WebSocketMessageType::Close)
      {
        this->OnClose(msg);
      }
      else if (msg->type == ix::WebSocketMessageType::Error)
      {
        this->OnError(msg);
      }
      else if (msg->type == ix::WebSocketMessageType::Ping)
      {
        this->OnPing(msg);
      }
      else if(msg->type == ix::WebSocketMessageType::Pong)
      {
        this->OnPong(msg);
      }
    }
  };
};

#endif
