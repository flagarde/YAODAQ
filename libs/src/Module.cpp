#include "Module.hpp"

#include "sinks/ansicolor_sink.h"
#include "sinks/ostream_sink.h"
#include "sinks/stdout_color_sinks.h"
#include "spdlog.h"
#include "toml.hpp"

Configuration Module::m_Config = Configuration();

void Module::setConfigFile(const std::string& file)
{
  m_Config.setFileName(file);
}

std::string Module::getName()
{
  return m_Name;
}
std::string Module::getType()
{
  return m_Type;
}

ix::WebSocketSendInfo Module::sendBinary(Message& message)
{
  message.setFrom(m_Type + "/" + m_Name);
  return m_WebsocketClient.sendBinary(message.get());
}

ix::WebSocketSendInfo Module::sendText(Message& message)
{
  message.setFrom(m_Type + "/" + m_Name);
  return m_WebsocketClient.sendText(message.get());
}

Module::Module(const std::string& name, const std::string& type): m_Type(type), m_Name(name)
{
  spdlog::sinks_init_list sink_list = {std::make_shared<spdlog::sinks::stdout_color_sink_mt>()};
  m_Logger                          = std::make_shared<spdlog::logger>(m_Type + "/" + m_Name, std::begin(sink_list), std::end(sink_list));
  m_WebsocketClient.setExtraHeader("Key", "///" + m_Type + "/" + m_Name);
  m_WebsocketClient.setOnMessageCallback(m_CallBack);
  m_WebsocketClient.start();
}

void Module::DoDoConnect() {}

void Module::DoDoDisconnect() {}

void Module::verifyParameters()
{
  std::cout << "Module VerifyParameters" << std::endl;
}

void Module::LoadConfig()
{
  m_Config.parse();
  m_Conf = m_Config.getConfig(m_Name);
  printParameters();
  this->verifyParameters();
}

void Module::printParameters()
{
  std::cout << m_Conf << std::endl;
}

Module::~Module() {}

void Module::Initialize()
{
  try
  {
    LoadConfig();
    DoInitialize();
    sendState(States::INITIALIZED);
    m_Logger->trace("Trace");
    m_Logger->info("info");
    m_Logger->debug("debug");
    m_Logger->warn("warn");
    m_Logger->error("error");
    m_Logger->critical("critical");
  }
  catch(const Exception& error)
  {
    m_Logger->error(error.what());
    throw;
  }
}

void Module::Connect()
{
  try
  {
    DoDoConnect();
    sendState(States::CONNECTED);
  }
  catch(const Exception& error)
  {
    m_Logger->error(error.what());
    throw;
  }
}

void Module::Configure()
{
  try
  {
    DoConfigure();
    sendState(States::CONFIGURED);
  }
  catch(const Exception& error)
  {
    m_Logger->error(error.what());
    throw;
  }
}

void Module::Start()
{
  try
  {
    DoStart();
    sendState(States::STARTED);
  }
  catch(const Exception& error)
  {
    m_Logger->error(error.what());
    throw;
  }
}

void Module::Pause()
{
  try
  {
    DoPause();
    sendState(States::PAUSED);
  }
  catch(const Exception& error)
  {
    m_Logger->error(error.what());
    throw;
  }
}

void Module::Stop()
{
  try
  {
    DoStop();
    sendState(States::STOPED);
  }
  catch(const Exception& error)
  {
    m_Logger->error(error.what());
    throw;
  }
}

void Module::Clear()
{
  try
  {
    DoClear();
    sendState(States::CLEARED);
  }
  catch(const Exception& error)
  {
    m_Logger->error(error.what());
    throw;
  }
}

void Module::Disconnect()
{
  try
  {
    DoDoDisconnect();
    sendState(States::DISCONNECTED);
  }
  catch(const Exception& error)
  {
    m_Logger->error(error.what());
    throw;
  }
}

void Module::Release()
{
  try
  {
    DoRelease();
    sendState(States::RELEASED);
  }
  catch(const Exception& error)
  {
    m_Logger->error(error.what());
    throw;
  }
}

void Module::Quit()
{
  try
  {
    DoQuit();
    sendState(States::QUITED);
  }
  catch(const Exception& error)
  {
    m_Logger->error(error.what());
    throw;
  }
}

void Module::sendState(const States& m_state)
{
  State state(m_state);
  sendText(state);
}

void Module::DoInitialize()
{
  std::cout << "Initialize" << std::endl;
}

void Module::DoConnect()
{
  std::cout << "Connect" << std::endl;
}

void Module::DoConfigure()
{
  std::cout << "Configure" << std::endl;
}

void Module::DoStart()
{
  std::cout << "Start" << std::endl;
}

void Module::DoPause()
{
  std::cout << "Pause" << std::endl;
}

void Module::DoStop()
{
  std::cout << "Stop" << std::endl;
}

void Module::DoClear()
{
  std::cout << "Clear" << std::endl;
}

void Module::DoDisconnect()
{
  std::cout << "Disconnect" << std::endl;
}

void Module::DoRelease()
{
  std::cout << "Release" << std::endl;
}

void Module::DoQuit()
{
  std::cout << "Quit" << std::endl;
}

void Module::DoOnAction(Message& message)
{
  if(message.getContent() == "INITIALIZE") { Initialize(); }
  else if(message.getContent() == "CONNECT")
  {
    Connect();
  }
  else if(message.getContent() == "CONFIGURE")
  {
    Configure();
  }
  else if(message.getContent() == "START")
  {
    Start();
  }
  else if(message.getContent() == "PAUSE")
  {
    Pause();
  }
  else if(message.getContent() == "STOP")
  {
    Stop();
  }
  else if(message.getContent() == "CLEAR")
  {
    Clear();
  }
  else if(message.getContent() == "DISCONNECT")
  {
    Disconnect();
  }
  else if(message.getContent() == "RELEASE")
  {
    Release();
  }
  else if(message.getContent() == "QUIT")
  {
    Quit();
  }
}

void Module::DoOnMessage(const ix::WebSocketMessagePtr& msg)
{
  Message message;
  message.parse(msg->str);
  if(message.getType() == "Action") { DoOnAction(message); }
  else
    OnMessage(msg);
}

void Module::OnOpen(const ix::WebSocketMessagePtr& msg)
{
  m_Logger->info("Handshake Headers :");
  for(auto it: msg->openInfo.headers) { m_Logger->info("\t{0}:{1}", it.first, it.second); }
  m_Logger->info("");
}

void Module::OnClose(const ix::WebSocketMessagePtr& msg)
{
  // The server can send an explicit code and reason for closing.
  // This data can be accessed through the closeInfo object.
  if(ix::WebSocketCloseConstants::kInternalErrorCode) { throw Exception(STATUS_CODE_ALREADY_PRESENT, msg->closeInfo.reason); }
  spdlog::info("{}", msg->closeInfo.code);
  spdlog::info("{}", msg->closeInfo.reason);
}

void Module::OnPong(const ix::WebSocketMessagePtr& msg)
{
  m_Logger->info("Pong data : {}", msg->str);
}

void Module::OnPing(const ix::WebSocketMessagePtr& msg)
{
  m_Logger->info("Ping data : {}", msg->str);
}

void Module::OnMessage(const ix::WebSocketMessagePtr& msg)
{
  m_Logger->info("{}", msg->str);
}

void Module::OnError(const ix::WebSocketMessagePtr& msg)
{
  m_Logger->error("Error : {}", msg->errorInfo.reason);
  m_Logger->error("#retries : {}", msg->errorInfo.retries);
  m_Logger->error("Wait time(ms) : {}", msg->errorInfo.wait_time);
  m_Logger->error("HTTP Status : {}", msg->errorInfo.http_status);
}
