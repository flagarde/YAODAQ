#pragma once

#include "Identifier.hpp"
#include "Interrupt.hpp"
#include "Classes.hpp"

#include "ConfigurationLoader.hpp"
#include "Message.hpp"
#include "States.hpp"
#include "StatusCode.hpp"
#include "WebSocketClient.hpp"
#include "spdlog/spdlog.h"

#include <iostream>
#include <mutex>
#include <string>
#include <thread>

//#ifdef SPDLOG_WCHAR_TO_UTF8_SUPPORT
//using yaodaq_string_view = spdlog::wstring_view_t;
//#else
using yaodaq_string_view = spdlog::string_view_t;
//#endif

class Module
{
public:
  Module(const std::string& name = "", const std::string& type = "Module", const yaodaq::CLASS& _class = yaodaq::CLASS::Module);
  virtual ~Module() = default;

  int loop();
  void skipConfigFile();

  void setURL(const std::string&);

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
  void                                   LoopOnStart();
  void                                   LoopOnPause();
  std::string                            getStateString();
  States                                 getState();
  std::string                            getName();
  std::string                            getType();
  void                                   setName(const std::string& name);
  static void                            setConfigFile(const std::string&);
  void                                   printParameters();
  void                                   stopListening();
  void                                   startListening();
  void                                   send(Message& message)
  {
    message.setFrom(m_Type+"/"+m_Name);
    m_WebsocketClient.send(message.get());
  }
  void                                   sendText(Message& message)
  {
    message.setFrom(m_Type+"/"+m_Name);
    m_WebsocketClient.sendText(message.get());
  }
  void                                   sendBinary(Message& message)
  {
    message.setFrom(m_Type+"/"+m_Name);
    m_WebsocketClient.sendBinary(message.get());
  }
  template<typename... Args> inline void sendTrace(yaodaq_string_view fmt, const Args&... args)
  {
    fmt::memory_buffer buf;
    format_to(buf, fmt, args...);
    Trace trace(buf.data());
    sendText(trace);
    m_Logger->trace(buf.data());
  }
  template<typename... Args> inline void sendDebug(yaodaq_string_view fmt, const Args&... args)
  {
    fmt::memory_buffer buf;
    format_to(buf, fmt, args...);
    Debug debug(buf.data());
    sendText(debug);
    m_Logger->debug(buf.data());
  }
  template<typename... Args> inline void sendInfo(yaodaq_string_view fmt, const Args&... args)
  {
    fmt::memory_buffer buf;
    format_to(buf, fmt, args...);
    Info info(buf.data());
    sendText(info);
    m_Logger->info(buf.data());
  }
  template<typename... Args> inline void sendWarning(yaodaq_string_view fmt, const Args&... args)
  {
    fmt::memory_buffer buf;
    format_to(buf, fmt, args...);
    Warning warning(buf.data());
    sendText(warning);
    m_Logger->warn(buf.data());
  }
  /** Work aroung for Exception message taken by what() **/
  inline void sendError(const char* err)
  {
    Error error(err);
    sendText(error);
    m_Logger->error(err);
  }
  template<typename... Args> inline void sendError(yaodaq_string_view fmt, const Args&... args)
  {
    fmt::memory_buffer buf;
    format_to(buf, fmt, args...);
    Error error(buf.data());
    sendText(error);
    m_Logger->error(buf.data());
  }
  template<typename... Args> inline void sendCritical(yaodaq_string_view fmt, const Args&... args)
  {
    fmt::memory_buffer buf;
    format_to(buf, fmt, args...);
    Critical critical(buf.data());
    sendText(critical);
    m_Logger->critical(buf.data());
  }
  void sendData(const std::string& dat) { Data data(dat); sendBinary(data); }
  void sendData(const Json::Value& dat) { Data data(dat); sendBinary(data); }

protected:
  static ConfigurationLoader            m_Config;
  virtual void                    OnOpen(const ix::WebSocketMessagePtr& msg);
  virtual void                    OnClose(const ix::WebSocketMessagePtr& msg);
  virtual void                    OnPong(const ix::WebSocketMessagePtr& msg);
  virtual void                    OnPing(const ix::WebSocketMessagePtr& msg);
  virtual void                    OnMessage(const ix::WebSocketMessagePtr& msg);
  virtual void                    OnError(const ix::WebSocketMessagePtr& msg);
  virtual void                    OnCommand(Command& command);
  void                            LoadConfig();
  virtual void                    verifyParameters();
  toml::value                     m_Conf{""};
  std::string                     m_Name{"Unknown"};
  std::string                     m_Type{"Unknown"};
  std::shared_ptr<spdlog::logger> m_Logger{nullptr};

private:
  void signalMessage();
  yaodaq::Identifier m_Identifier;
  yaodaq::Interrupt  m_Interrupt;
  bool            m_UseConfigFile{true};
  void            DoDoLoopOnStart();
  void            DoDoLoopOnPause();
  volatile States m_State{States::UNINITIALIZED};
  std::thread     m_LoopOnStart;
  std::thread     m_LoopOnPause;
  std::mutex      m_Mutex;
  bool            m_LoopOnStartUsed{false};
  bool            m_LoopOnPauseUsed{false};
  Module() = delete;
  virtual void                                        DoInitialize();
  virtual void                                        CallBoardConnect(){};
  virtual void                                        DoConfigure();
  virtual void                                        DoStart();
  virtual void                                        DoPause();
  virtual void                                        DoStop();
  virtual void                                        DoClear();
  virtual void                                        CallBoardDisconnect(){};
  virtual void                                        DoRelease();
  virtual void                                        DoQuit();
  virtual void                                        DoLoopOnStart();
  virtual void                                        DoLoopOnPause();
  virtual void                                        DoAtFirstStart();
  bool                                                m_IsFirstStart{true};
  void                                                DoOnAction(const Message& message);
  void                                                DoOnCommand(const Message& message);
  virtual void                                        DoOnData(const Data& data);
  void                                                sendState();
  void                                                setState(const States& state);
  void                                                DoOnMessage(const ix::WebSocketMessagePtr& msg);
  yaodaq::WebSocketClient                                     m_WebsocketClient;
  std::function<void(const ix::WebSocketMessagePtr&)> m_CallBack;
};
