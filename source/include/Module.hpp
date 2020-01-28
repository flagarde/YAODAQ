#ifndef MODULE_HPP
#define MODULE_HPP

#include <iostream>
#include <sstream>
#include <string>
#include "Message.hpp"
#include "Info.hpp"
#include "Status.hpp"
#include "Log.hpp"
#include "WebsocketClient.hpp"
#include "Configuration.hpp"
#include "spdlog.h"
#include "Error.hpp"

class Module 
{
public :
  Module(const std::string& type="Unknown",const std::string& name="Unknown");
  virtual ~Module();
  void Initialize();
  void Connect();
  void Configure();
  void Start();
  void Pause();
  void Stop();
  void Clear();
  void Disconnect();
  void Release();
  void Quit();
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
  static void setConfigFile(const std::string&);
  void printParameters();
protected:
  virtual void DoInitialize();
  virtual void DoConnect();
  virtual void DoConfigure();
  virtual void DoStart();
  virtual void DoPause();
  virtual void DoStop();
  virtual void DoClear();
  virtual void DoDisconnect();
  virtual void DoRelease();
  virtual void DoQuit();
  virtual void LoadConfig();
  virtual void DoDoConnect();
  virtual void DoDoDisconnect();
  toml::value m_Conf;
  static Configuration m_Config;
  std::string m_Name{"Unknown"};
  std::string m_Type{"Unknown"};
  std::shared_ptr<spdlog::logger> m_Logger{nullptr};
  void sendLog(const std::string& log="");
private:
  std::ostringstream oss;
  void DoOnStatus(Message& message);
  void sendStatus(const std::string&);
  void DoOnMessage(const ix::WebSocketMessagePtr& msg);
  WebsocketClient m_WebsocketClient;
  static std::shared_ptr<spdlog::sinks::sink>  console_sink;
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
