#pragma once

#include "Configuration.hpp"
#include "Message.hpp"
#include "States.hpp"
#include "WebsocketClient.hpp"
#include "spdlog.h"
#include "StatusCode.hpp"
#include <iostream>
#include <string>
//#ifdef SPDLOG_WCHAR_TO_UTF8_SUPPORT
//using yaodaq_string_view = spdlog::wstring_view_t;
//#else
using yaodaq_string_view = spdlog::string_view_t;
//#endif

class Module
{
public:
  Module(const std::string& name = "", const std::string& type = "Module");
  virtual ~Module();
  void                                   Initialize();
  void                                   Connect();
  void                                   Configure();
  void                                   Start();
  void                                   Pause();
  void                                   Stop();
  void                                   Clear();
  void                                   Disconnect();
  void                                   Release();
  void                                   Quit();
  std::string                            getStateString();
  States                                 getState();
  std::string                            getName();
  std::string                            getType();
  void                                   setName(const std::string& name);
  static void                            setConfigFile(const std::string&);
  void                                   printParameters();
  void                                   stopListening();
  void                                   startListening();
  template<typename... Args> inline void sendTrace(yaodaq_string_view fmt, const Args&... args)
  {
    fmt::memory_buffer buf;
    format_to(buf, fmt, args...);
    Trace trace(buf.data());
    m_WebsocketClient.sendText(trace.get());
    m_Logger->trace(buf.data());
  }
  template<typename... Args> inline void sendDebug(yaodaq_string_view fmt, const Args&... args)
  {
    fmt::memory_buffer buf;
    format_to(buf, fmt, args...);
    Debug debug(buf.data());
    m_WebsocketClient.sendText(debug.get());
    m_Logger->debug(buf.data());
  }
  template<typename... Args> inline void sendInfo(yaodaq_string_view fmt, const Args&... args)
  {
    fmt::memory_buffer buf;
    format_to(buf, fmt, args...);
    Info info(buf.data());
    m_WebsocketClient.sendText(info.get());
    m_Logger->info(buf.data());
  }
  template<typename... Args> inline void sendWarning(yaodaq_string_view fmt, const Args&... args)
  {
    fmt::memory_buffer buf;
    format_to(buf, fmt, args...);
    Warning warning(buf.data());
    m_WebsocketClient.sendText(warning.get());
    m_Logger->warn(buf.data());
  }
  /** Work aroung for Exception message taken by what() **/
  inline void sendError(const char* err)
  {
    Error error(err);
    m_WebsocketClient.sendText(error.get());
    m_Logger->error(err);
  }
  template<typename... Args> inline void sendError(yaodaq_string_view fmt, const Args&... args)
  {
    fmt::memory_buffer buf;
    format_to(buf, fmt, args...);
    Error error(buf.data());
    m_WebsocketClient.sendText(error.get());
    m_Logger->error(buf.data());
  }
  template<typename... Args> inline void sendCritical(yaodaq_string_view fmt, const Args&... args)
  {
    fmt::memory_buffer buf;
    format_to(buf, fmt, args...);
    Critical critical(buf.data());
    m_WebsocketClient.sendText(critical.get());
    m_Logger->critical(buf.data());
  }

protected:
  virtual void                    OnOpen(const ix::WebSocketMessagePtr& msg);
  virtual void                    OnClose(const ix::WebSocketMessagePtr& msg);
  virtual void                    OnPong(const ix::WebSocketMessagePtr& msg);
  virtual void                    OnPing(const ix::WebSocketMessagePtr& msg);
  virtual void                    OnMessage(const ix::WebSocketMessagePtr& msg);
  virtual void                    OnError(const ix::WebSocketMessagePtr& msg);
  virtual void                    OnCommand(Command& command);
  void                            LoadConfig();
  virtual void                    verifyParameters();
  toml::value                     m_Conf;
  static Configuration            m_Config;
  std::string                     m_Name{"Unknown"};
  std::string                     m_Type{"Unknown"};
  std::shared_ptr<spdlog::logger> m_Logger{nullptr};
  States                          m_State{States::UNINITIALIZED};

private:
  Module() = delete;
  virtual void                                        DoInitialize();
  virtual void                                        CallModuleConnect(){};
  virtual void                                        DoConfigure();
  virtual void                                        DoStart();
  virtual void                                        DoPause();
  virtual void                                        DoStop();
  virtual void                                        DoClear();
  virtual void                                        CallModuleDisconnect(){};
  virtual void                                        DoRelease();
  virtual void                                        DoQuit();
  void                                                DoOnAction(const Message& message);
  void                                                DoOnCommand(const Message& message);
  void                                                sendState();
  void                                                setState(const States& state);
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
