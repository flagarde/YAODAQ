#ifndef MODULE_HPP
#define MODULE_HPP

#include <iostream>
#include <sstream>
#include <string>
#include "Message.hpp"
#include "Infos.hpp"
#include "WebsocketClient.hpp"

class Module 
{
public :
  Module( const std::string& type="Unknown",const std::string& name="Unknown");
  ~Module();
  virtual void Initialize();
  virtual void Connect();
  virtual void Configure();
  virtual void Start();
  virtual void Pause();
  virtual void Stop();
  virtual void Clear();
  virtual void Disconnect();
  virtual void Release();
  std::string getName();
  std::string getType();
  virtual void OnOpen(const std::string&);
  virtual void OnClose(const std::string&);
  virtual void OnPong(const std::string&);
  virtual void OnPing(const std::string&);
  virtual void OnMessage(const std::string&);
  
  ix::WebSocketSendInfo sendBinary(Message& message);
  ix::WebSocketSendInfo sendText(Message& message);
  
  
  
private:
  WebsocketClient m_WebsocketClient;
  std::string m_Name{"Unknown"};
  std::string m_Type{"Unknown"};
  std::function<void(const ix::WebSocketMessagePtr&)> m_CallBack{[this](const ix::WebSocketMessagePtr& msg)
  {
    if (msg->type == ix::WebSocketMessageType::Message)
    {
      //this->OnMessage(msg->str);
      std::cout << msg->str << std::endl;
      std::cout << getName() << std::endl;
    }
    
    else if (msg->type == ix::WebSocketMessageType::Open)
    {
      std::cout << "send greetings" << std::endl;
      
      // Headers can be inspected (pairs of string/string)
      std::cout << "Handshake Headers:" << std::endl;
      for (auto it : msg->openInfo.headers)
      {
        std::cout << it.first << ": " << it.second << std::endl;
      }
    }
    else if (msg->type == ix::WebSocketMessageType::Close)
    {
      std::cout << "disconnected" << std::endl;
      
      // The server can send an explicit code and reason for closing.
      // This data can be accessed through the closeInfo object.
      std::cout << msg->closeInfo.code << std::endl;
      std::cout << msg->closeInfo.reason << std::endl;
    }
    else if (msg->type == ix::WebSocketMessageType::Error)
    {
      std::stringstream ss;
      ss << "Error: "         << msg->errorInfo.reason      << std::endl;
      ss << "#retries: "      << msg->errorInfo.retries     << std::endl;
      ss << "Wait time(ms): " << msg->errorInfo.wait_time   << std::endl;
      ss << "HTTP Status: "   << msg->errorInfo.http_status << std::endl;
      std::cout << ss.str() << std::endl;
    }
    else if (msg->type == ix::WebSocketMessageType::Ping)
    {
      std::cout << "pong data: " << msg->str << std::endl;
    }
    else if(msg->type == ix::WebSocketMessageType::Pong)
    {
      
    }
  }};
};

#endif
