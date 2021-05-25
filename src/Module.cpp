#include "Module.hpp"

#include <thread>

#include "Classes.hpp"

#include "Exception.hpp"
#include "Message.hpp"
#include "StatusCode.hpp"
#include "magic_enum.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace yaodaq
{

void Module::setURL(const std::string& url)
{
  m_WebsocketClient.setUrl(url);
  URLIsSet=true;
}

Module::Module(const std::string& name, const std::string& type, const yaodaq::CLASS& _class): MessageHandlerClient(Identifier(_class,type,name))
{
  m_LoggerHandler.setName(m_Identifier.getIdentifier());
  m_WebsocketClient.setHeaderKey("Key", "///" + getType() + "/" + getName());
  m_CallBack = {[this](const ix::WebSocketMessagePtr& msg) {
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
}

int Module::loop()
{
  std::thread startListening(&Module::startListening,this);
  startListening.detach();
  onRaisingSignal();
  stopListening();
  return 0;
}




ConfigurationLoader Module::m_Config = ConfigurationLoader();

void Module::setConfigFile(const std::string& file)
{
  m_Config.setFileName(file);
}


void Module::stopListening()
{
  m_WebsocketClient.stop();
  while(m_WebsocketClient.getReadyState()!=ix::ReadyState::Closed) std::this_thread::sleep_for(std::chrono::microseconds(10));
}

void Module::startListening()
{
  m_LoggerHandler.addSink(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
  logger()->info("Listening on {}.",m_WebsocketClient.getUrl());
  if(!URLIsSet)
  {
    setURL(yaodaq::GeneralParameters::getURL());
  }
  m_WebsocketClient.start();
  while(m_WebsocketClient.getReadyState()!=ix::ReadyState::Open) std::this_thread::sleep_for(std::chrono::microseconds(10));
}

std::string Module::getName()
{
  return m_Identifier.getName();
}
std::string Module::getType()
{
  return m_Identifier.getType();
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



void Module::verifyParameters() {}

void Module::LoadConfig()
{
  m_Config.parse();
  m_Conf = m_Config.getConfig(getName());
  verifyParameters();
}

void Module::printParameters()
{
  logger()->info("Parameters :\n{}", toml::format(m_Conf));
}

void Module::sendState()
{
  State state(m_State);
  m_WebsocketClient.sendBinary(state.get());
  logger()->warn(state.get());
}

void Module::Initialize()
{
  try
  {
    if(m_UseConfigFile)LoadConfig();
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
    if(m_UseConfigFile)
    {
      m_Config.reloadParameters(getName());
      m_Conf = m_Config.getConfig(getName());
    }
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
    if(m_LoopOnPauseUsed == true)
    {
      m_LoopOnPause.join();
      m_LoopOnPauseUsed = false;
    }
    if(m_IsFirstStart == true)
    {
      DoAtFirstStart();
      m_IsFirstStart = false;
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
    if(m_LoopOnStartUsed == true)
    {
      m_LoopOnStart.join();
      m_LoopOnStartUsed = false;
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
    setState(States::STOPPED);
    m_IsFirstStart = true;
    if(m_LoopOnStartUsed == true)
    {
      m_LoopOnStart.join();
      m_LoopOnStartUsed = false;
    }
    if(m_LoopOnPauseUsed == true)
    {
      m_LoopOnPause.join();
      m_LoopOnPauseUsed = false;
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
    CallBoardDisconnect();
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
  if(m_LoopOnStartUsed == false)
  {
    m_LoopOnStart     = std::thread(&Module::DoDoLoopOnStart, this);
    m_LoopOnStartUsed = true;
  }
}

void Module::LoopOnPause()
{
  if(m_LoopOnPauseUsed == false)
  {
    m_LoopOnPause     = std::thread(&Module::DoDoLoopOnPause, this);
    m_LoopOnPauseUsed = true;
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
  while(getState() == States::STARTED) { DoLoopOnStart(); }
}

void Module::DoDoLoopOnPause()
{
  while(getState() == States::PAUSED) { DoLoopOnPause(); }
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
    if((m_State==States::UNINITIALIZED || m_State==States::INITIALIZED|| m_State==States::RELEASED) && action.value()== Actions::INITIALIZE) Initialize();
    else if((m_State==States::INITIALIZED || m_State==States::DISCONNECTED||m_State==States::CONNECTED) && action.value()== Actions::CONNECT ) Connect();
    else if((m_State==States::CONNECTED || m_State==States::CLEARED || m_State==States::CONFIGURED ) && action.value()== Actions::CONFIGURE ) Configure();
    else if((m_State==States::CONFIGURED || m_State==States::STOPPED || m_State==States::STARTED || m_State==States::PAUSED) && action.value()== Actions::START ) Start();
    else if((m_State==States::STARTED || m_State==States::PAUSED) && action.value()== Actions::PAUSE) Pause();
    else if((m_State==States::STARTED || m_State==States::PAUSED || m_State==States::STOPPED)  && action.value()== Actions::STOP) Stop();
    else if((m_State==States::STOPPED || m_State==States::CONFIGURED || m_State==States::CLEARED) && action.value()== Actions::CLEAR) Clear();
    else if((m_State==States::CLEARED || m_State==States::CONNECTED|| m_State==States::DISCONNECTED) && action.value()== Actions::DISCONNECT) Disconnect();
    else if((m_State==States::DISCONNECTED || m_State==States::INITIALIZED|| m_State==States::RELEASED) && action.value()== Actions::RELEASE) Release();
    else if(m_State==States::RELEASED && action.value()== Actions::QUIT) Quit();
    else sendError("Module/Board \"{}\" in state {}. Cannot perform action {}",getName(),std::string(magic_enum::enum_name(m_State)),std::string(magic_enum::enum_name(action.value())));
  }
}

void Module::DoOnData(const Data& data) {}

void Module::DoOnMessage(const ix::WebSocketMessagePtr& msg)
{
  Message message;
  message.parse(msg->str);
  if(message.getType() == "Action") DoOnAction(message);
  else if(message.getType() == "Command")
    DoOnCommand(message);
  else if(message.getType() == "Data")
  {
    Data data;
    data.parse(message.get());
    DoOnData(data);
  }
  else
    OnMessage(msg);
}

void Module::skipConfigFile()
{
  m_UseConfigFile=false;
}

void Module::OnOpen(const ix::WebSocketMessagePtr& msg)
{
  logger()->info("Handshake Headers :");
  for(auto it: msg->openInfo.headers) { logger()->info("\t{0}:{1}", it.first, it.second); }
  logger()->info("");
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
  else
  {
    logger()->info("{}", msg->closeInfo.code);
    logger()->info("{}", msg->closeInfo.reason);
  }
}

void Module::OnPong(const ix::WebSocketMessagePtr& msg)
{
  logger()->info("Pong data : {}", msg->str);
}

void Module::OnPing(const ix::WebSocketMessagePtr& msg)
{
  logger()->info("Ping data : {}", msg->str);
}

void Module::OnMessage(const ix::WebSocketMessagePtr& msg)
{
  logger()->info("{}", msg->str);
}

void Module::OnError(const ix::WebSocketMessagePtr& msg)
{
  logger()->error("Error : {}", msg->errorInfo.reason);
  logger()->error("#retries : {}", msg->errorInfo.retries);
  logger()->error("Wait time(ms) : {}", msg->errorInfo.wait_time);
  logger()->error("HTTP Status : {}", msg->errorInfo.http_status);
}

};
