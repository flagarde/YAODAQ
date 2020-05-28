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
  std::lock_guard<std::mutex> guard(m_Mutex);
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
  m_CallBack={[this](const ix::WebSocketMessagePtr& msg) {
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
  m_WebsocketClient.setOnMessageCallback(m_CallBack);
  startListening();
}

void Module::verifyParameters() {}

void Module::reparseModules()
{
  m_Config.reparseModule();
  m_Conf = m_Config.getConfig(getName());
}

void Module::LoadConfig()
{
  m_Config.parse();
  m_Conf = m_Config.getConfig(m_Name);
  std::cout<<m_Conf<<std::endl;
  verifyParameters();
}

void Module::printParameters()
{
  m_Logger->info("Parameters :\n{}",toml::format(m_Conf));
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
    CallBoardConnect();
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
    setState(States::STARTED);
    if(m_LoopOnPauseUsed==true)
    {
      m_LoopOnPause.join();
      m_LoopOnPauseUsed=false;
    }
    if(m_IsFirstStart==true)
    {
      DoAtFirstStart();
      m_IsFirstStart=false;
    }
    DoStart();
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
    setState(States::PAUSED);
    if(m_LoopOnStartUsed==true)
    {
      m_LoopOnStart.join();
      m_LoopOnStartUsed=false;
    }
    DoPause();
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
    setState(States::STOPED);
    m_IsFirstStart=true;
    if(m_LoopOnStartUsed==true)
    {
      m_LoopOnStart.join();
      m_LoopOnStartUsed=false;
    }
    if(m_LoopOnPauseUsed==true)
    {
      m_LoopOnPause.join();
      m_LoopOnPauseUsed=false;
    }
    DoStop();
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
    std::cout<<"Module Disconnect"<<std::endl;
    CallBoardDisconnect();
    setState(States::DISCONNECTED);
    sendState();
    std::cout<<"Module Disconnect"<<std::endl;
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
    m_Config.clear();
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

void Module::LoopOnStart()
{
  if(m_LoopOnStartUsed==false)
  {
    m_LoopOnStart=std::thread(&Module::DoDoLoopOnStart,this);
    m_LoopOnStartUsed=true;
  }
}

void Module::LoopOnPause()
{
  if(m_LoopOnPauseUsed==false)
  {
    m_LoopOnPause=std::thread(&Module::DoDoLoopOnPause,this);
    m_LoopOnPauseUsed=true;
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

void Module::DoLoopOnStart(){};
void Module::DoLoopOnPause(){};
void Module::DoAtFirstStart(){};

void Module::DoDoLoopOnStart() 
{
  while(getState()==States::STARTED)
  {
    DoLoopOnStart();
  }
}

void Module::DoDoLoopOnPause() 
{
  while(getState()==States::PAUSED)
  {
    DoLoopOnPause();
  }
}


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
