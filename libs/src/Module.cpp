#include "Module.hpp"

#include "Exception.hpp"
#include "Message.hpp"
#include "StatusCode.hpp"
#include "magic_enum.hpp"
#include "sinks/ansicolor_sink.h"
#include "sinks/ostream_sink.h"
#include "sinks/stdout_color_sinks.h"
#include "spdlog.h"

bool Module::m_HaveToReloadConfigModules=true;
bool Module::m_HaveToReloadConfig=true;

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

Module::Module(const std::string& name, const std::string& type): m_Type(type), m_Name(name)
{
  spdlog::sinks_init_list sink_list = {std::make_shared<spdlog::sinks::stdout_color_sink_mt>()};
  m_Logger                          = std::make_shared<spdlog::logger>(m_Type + "/" + m_Name, std::begin(sink_list), std::end(sink_list));
  m_WebsocketClient.setExtraHeader("Key", "///" + m_Type + "/" + m_Name);
  m_WebsocketClient.setOnMessageCallback(m_CallBack);
  startListening();
}

void Module::verifyParameters() {}

void Module::staticReparseModules()
{
  if(m_HaveToReloadConfigModules==true)
  {
    m_Config.reparseModule();
    m_HaveToReloadConfigModules=false;
  }
}

void Module::reparseModules()
{
  staticReparseModules();
  m_Conf = m_Config.getConfig(m_Name);
}

void Module::LoadConfig()
{
  m_Config.parse();
  m_Conf = m_Config.getConfig(m_Name);
  verifyParameters();
}

void Module::printParameters()
{
  m_Logger->info(m_Conf.as_string());
}

Module::~Module()
{
  stopListening();
}

void Module::sendState()
{
  State state(m_State);
  m_WebsocketClient.sendBinary(state.get());
  m_Logger->warn(state.get());
}

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
    sendError(error.what());
    stopListening();
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
    sendError(error.what());
    stopListening();
    throw;
  }
}

void Module::Configure()
{
  try
  {
    reparseModules();
    DoConfigure();
    setState(States::CONFIGURED);
    sendState();
  }
  catch(const Exception& error)
  {
    sendError(error.what());
    stopListening();
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
    sendError(error.what());
    stopListening();
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
    sendError(error.what());
    stopListening();
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
    sendError(error.what());
    stopListening();
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
    sendError(error.what());
    stopListening();
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
    sendError(error.what());
    stopListening();
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
    sendError(error.what());
    stopListening();
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
    sendError(error.what());
    stopListening();
    throw;
  }
}

void Module::DoInitialize() {}

void Module::DoConfigure() {}

void Module::DoStart() {}

void Module::DoPause() {}

void Module::DoStop() {}

void Module::DoClear() {}

void Module::DoRelease() {}

void Module::DoQuit() {}

void Module::OnCommand(Command& command) {}

void Module::DoOnCommand(const Message& command)
{
  Command m_command;
  m_command.parse(command.get());
  if(m_command.getCommand() == "getState") sendState();
}

void Module::DoOnAction(const Message& message)
{
  auto action = magic_enum::enum_cast<Actions>(message.getContent());
  if(action.has_value())
  {
    switch(action.value())
    {
      case Actions::INITIALIZE:
      {
        Initialize();
        break;
      }
      case Actions::CONNECT:
      {
        Connect();
        break;
      }
      case Actions::CONFIGURE:
      {
        m_HaveToReloadConfigModules=true;
        Configure();
        break;
      }
      case Actions::START:
      {
        Start();
        break;
      }
      case Actions::PAUSE:
      {
        Pause();
        break;
      }
      case Actions::STOP:
      {
        Stop();
        break;
      }
      case Actions::CLEAR:
      {
        Clear();
        break;
      }
      case Actions::DISCONNECT:
      {
        Disconnect();
        break;
      }
      case Actions::RELEASE:
      {
        Release();
        break;
      }
      case Actions::QUIT:
      {
        Quit();
        break;
      }
      default:
        break;
    }
  }
}

void Module::DoOnMessage(const ix::WebSocketMessagePtr& msg)
{
  Message message;
  message.parse(msg->str);
  if(message.getType() == "Action") { DoOnAction(message); }
  else if(message.getType() == "Command")
  {
    DoOnCommand(message);
  }
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
  if(msg->closeInfo.code == static_cast<int16_t>(StatusCode::ALREADY_PRESENT))
  {
    m_WebsocketClient.disableAutomaticReconnection();
    throw Exception(StatusCode::ALREADY_PRESENT, msg->closeInfo.reason);
  }
  m_Logger->info("{}", msg->closeInfo.code);
  m_Logger->info("{}", msg->closeInfo.reason);
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
