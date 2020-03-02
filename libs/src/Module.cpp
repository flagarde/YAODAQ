#include "Module.hpp"

#include "Interrupt.hpp"
#include "magic_enum.hpp"
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

void Module::setName(const std::string& name)
{
  m_Name = name;
}

void Module::stopListening()
{
  m_WebsocketClient.stop();
}

void Module::startListening()
{
  m_WebsocketClient.start();
}

std::string Module::getName()
{
  return m_Name;
}
std::string Module::getType()
{
  return m_Type;
}

void Module::setState(const States& state)
{
  m_State = state;
}

std::string Module::getStateString()
{
  return std::string(magic_enum::enum_name(m_State));
}

States Module::getState()
{
  return m_State;
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
}

void Module::verifyParameters() {}

void Module::LoadConfig()
{
  m_Config.parse();
  m_Conf = m_Config.getConfig(m_Name);
  verifyParameters();
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
    setState(States::INITIALIZED);
    sendState();
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
    CallModuleConnect();
    setState(States::CONNECTED);
    sendState();
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
    setState(States::CONFIGURED);
    sendState();
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
    setState(States::STARTED);
    sendState();
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
    setState(States::PAUSED);
    sendState();
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
    setState(States::STOPED);
    sendState();
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
    setState(States::CLEARED);
    sendState();
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
    CallModuleDisconnect();
    setState(States::DISCONNECTED);
    sendState();
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
    setState(States::RELEASED);
    sendState();
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
    setState(States::QUITED);
    sendState();
  }
  catch(const Exception& error)
  {
    m_Logger->error(error.what());
    throw;
  }
}

void Module::sendState()
{
  State state(m_State);
  sendText(state);
}

void Module::DoInitialize()
{
  std::cout << "Initialize" << std::endl;
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
