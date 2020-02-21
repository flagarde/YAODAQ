#pragma once

#include "Configuration.hpp"
#include "Error.hpp"
#include "Message.hpp"
#include "WebsocketClient.hpp"
#include "spdlog.h"

#include <iostream>
#include <sstream>
#include <string>

class Module {
public:
  Module(const std::string &name = "", const std::string &type = "Module");
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
  virtual void OnOpen(const ix::WebSocketMessagePtr &msg);
  virtual void OnClose(const ix::WebSocketMessagePtr &msg);
  virtual void OnPong(const ix::WebSocketMessagePtr &msg);
  virtual void OnPing(const ix::WebSocketMessagePtr &msg);
  virtual void OnMessage(const ix::WebSocketMessagePtr &msg);
  virtual void OnError(const ix::WebSocketMessagePtr &msg);
  ix::WebSocketSendInfo sendBinary(Message &message);
  ix::WebSocketSendInfo sendText(Message &message);
  static void setConfigFile(const std::string &);
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
  virtual void verifyParameters();
  toml::value m_Conf;
  static Configuration m_Config;
  std::string m_Name{"Unknown"};
  std::string m_Type{"Unknown"};
  std::shared_ptr<spdlog::logger> m_Logger{nullptr};

private:
  void DoOnStatus(Message &message);
  void sendStatus(const std::string &);
  void DoOnMessage(const ix::WebSocketMessagePtr &msg);
  WebsocketClient m_WebsocketClient;
  std::function<void(const ix::WebSocketMessagePtr &)> m_CallBack{
      [this](const ix::WebSocketMessagePtr &msg) {
        if (msg->type == ix::WebSocketMessageType::Message) {
          this->DoOnMessage(msg);
        } else if (msg->type == ix::WebSocketMessageType::Open) {
          this->OnOpen(msg);
        } else if (msg->type == ix::WebSocketMessageType::Close) {
          this->OnClose(msg);
        } else if (msg->type == ix::WebSocketMessageType::Error) {
          this->OnError(msg);
        } else if (msg->type == ix::WebSocketMessageType::Ping) {
          this->OnPing(msg);
        } else if (msg->type == ix::WebSocketMessageType::Pong) {
          this->OnPong(msg);
        }
      }};
};
