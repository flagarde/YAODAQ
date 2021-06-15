#include "Module.hpp"

#include <thread>

#include "Classes.hpp"

#include "Exception.hpp"
#include "Message.hpp"
#include "StatusCode.hpp"
#include "magic_enum.hpp"

namespace yaodaq
{

Module::Module(const std::string& name, const std::string& type, const yaodaq::CLASS& _class) : MessageHandlerClient(Identifier(_class,type,name))
{
  setOnMessageCallback(getMessageCallback());
  setHeaderKey("Key", getIdentifier().get());
  AddMethod("getState", GetHandle(&Module::getStateStr, *this));
  //AddMethod("sendState", GetHandle(&Module::sendState, *this)); not woeking WHY!!!!!!!!!!
}

int Module::loop()
{
  std::thread startListening(&Module::startListening,this);
  //startListening();
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
  stop();
  while(getReadyState()!=ix::ReadyState::Closed)
  {
    std::this_thread::sleep_for(std::chrono::microseconds(1000));
  }
  std::exit(0);
}

void Module::wait()
{
  while(getReadyState()!=ix::ReadyState::Open) std::this_thread::sleep_for(std::chrono::microseconds(1000));
}

void Module::startListening()
{
  std::thread start(&Module::start,this);
  start.detach();
 // start();
  logger()->info("Listening on {}.",getUrl());
  while(getReadyState()!=ix::ReadyState::Open) std::this_thread::sleep_for(std::chrono::microseconds(1000));
}

void Module::setState(const STATE& state)
{
  m_State = state;
}

STATE Module::getState()
{
  return m_State;
}

void Module::verifyParameters() {}

void Module::LoadConfig()
{
  m_Config.parse();
  m_Conf = m_Config.getConfig(getIdentifier().getName());
  verifyParameters();
}

void Module::printParameters()
{
  logger()->info("Parameters :\n{}", toml::format(m_Conf));
}

void Module::sendState()
{
  MessageHandlerClient::sendState(m_State);
}

void Module::Initialize()
{
  try
  {
    if(m_UseConfigFile)LoadConfig();
    DoInitialize();
    setState(STATE::Initialized);
    sendState();
  }
  catch(const Exception& exception)
  {
    error(exception.what());
    stopListening();
    throw;
  }
}

void Module::Connect()
{
  try
  {
    CallBoardConnect();
    setState(STATE::Connected);
    sendState();
  }
  catch(const Exception& exception)
  {
    error(exception.what());
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
      m_Config.reloadParameters(getIdentifier().getName());
      m_Conf = m_Config.getConfig(getIdentifier().getName());
    }
    DoConfigure();
    setState(STATE::Configured);
    sendState();
  }
  catch(const Exception& exception)
  {
    error(exception.what());
    stopListening();
    throw;
  }
}

void Module::Start()
{
  try
  {
    setState(STATE::Started);
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
  catch(const Exception& exception)
  {
    error(exception.what());
    stopListening();
    throw;
  }
}

void Module::Pause()
{
  try
  {
    setState(STATE::Paused);
    if(m_LoopOnStartUsed == true)
    {
      m_LoopOnStart.join();
      m_LoopOnStartUsed = false;
    }
    DoPause();
    sendState();
  }
  catch(const Exception& exception)
  {
    error(exception.what());
    stopListening();
    throw;
  }
}

std::string Module::getStateStr()
{
  sendState(); //SEEEEE AddMethod("sendState", GetHandle(&Module::sendState, *this));


  return std::string(magic_enum::enum_name(getState()));
}


void Module::Stop()
{
  try
  {
    setState(STATE::Stopped);
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
  catch(const Exception& exception)
  {
    error(exception.what());
    stopListening();
    throw;
  }
}

void Module::Clear()
{
  try
  {
    DoClear();
    setState(STATE::Cleared);
    sendState();
  }
  catch(const Exception& exception)
  {
    error(exception.what());
    stopListening();
    throw;
  }
}

void Module::Disconnect()
{
  try
  {
    CallBoardDisconnect();
    setState(STATE::Disconnected);
    sendState();
  }
  catch(const Exception& exception)
  {
    error(exception.what());
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
    setState(STATE::Released);
    sendState();
  }
  catch(const Exception& exception)
  {
    error(exception.what());
    stopListening();
    throw;
  }
}

void Module::Quit()
{
  try
  {
    DoQuit();
    setState(STATE::Quited);
    sendState();
  }
  catch(const Exception& exception)
  {
    error(exception.what());
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
  while(getState() == STATE::Started) { DoLoopOnStart(); }
}

void Module::DoDoLoopOnPause()
{
  while(getState() == STATE::Paused) { DoLoopOnPause(); }
}
/*
void Module::DoOnCommand(const Message& command)
{
  Command m_command;
  m_command.parse(command.get());
  if(m_command.getCommand() == "getState") sendState();
}*/

void Module::onAction(const Action& action)
{
  if(getIdentifier().getClass() != CLASS::Logger && getIdentifier().getClass() != CLASS::Controller)
  {
    switch(action.getAction())
    {
      case ACTION::Initialize :
      {
        if(m_State==STATE::Uninitialized || m_State==STATE::Initialized|| m_State==STATE::Released) Initialize();
        else error("Module/Board \"{}\" in state {}. Cannot perform action {}",getIdentifier().getName(),std::string(magic_enum::enum_name(m_State)),action.getActionStr());
        break;
      }
      case ACTION::Connect :
      {
        if(m_State==STATE::Initialized || m_State==STATE::Disconnected||m_State==STATE::Connected) Connect();
        else error("Module/Board \"{}\" in state {}. Cannot perform action {}",getIdentifier().getName(),std::string(magic_enum::enum_name(m_State)),action.getActionStr());
        break;
      }
      case ACTION::Configure :
      {
        if(m_State==STATE::Connected || m_State==STATE::Cleared || m_State==STATE::Configured ) Configure();
        else error("Module/Board \"{}\" in state {}. Cannot perform action {}",getIdentifier().getName(),std::string(magic_enum::enum_name(m_State)),action.getActionStr());
        break;
      }
      case ACTION::Start :
      {
        if(m_State==STATE::Configured || m_State==STATE::Stopped || m_State==STATE::Started || m_State==STATE::Paused) Start();
        else error("Module/Board \"{}\" in state {}. Cannot perform action {}",getIdentifier().getName(),std::string(magic_enum::enum_name(m_State)),action.getActionStr());
        break;
      }
      case ACTION::Pause :
      {
        if(m_State==STATE::Started || m_State==STATE::Paused) Pause();
        else error("Module/Board \"{}\" in state {}. Cannot perform action {}",getIdentifier().getName(),std::string(magic_enum::enum_name(m_State)),action.getActionStr());
        break;
      }
      case ACTION::Stop :
      {
        if(m_State==STATE::Started || m_State==STATE::Paused || m_State==STATE::Stopped) Stop();
        else error("Module/Board \"{}\" in state {}. Cannot perform action {}",getIdentifier().getName(),std::string(magic_enum::enum_name(m_State)),action.getActionStr());
        break;
      }
      case ACTION::Clear :
      {
        if(m_State==STATE::Stopped || m_State==STATE::Configured || m_State==STATE::Cleared) Clear();
        else error("Module/Board \"{}\" in state {}. Cannot perform action {}",getIdentifier().getName(),std::string(magic_enum::enum_name(m_State)),action.getActionStr());
        break;
      }
      case ACTION::Disconnect :
      {
        if(m_State==STATE::Cleared || m_State==STATE::Connected|| m_State==STATE::Disconnected) Disconnect();
        else error("Module/Board \"{}\" in state {}. Cannot perform action {}",getIdentifier().getName(),std::string(magic_enum::enum_name(m_State)),action.getActionStr());
        break;
      }
      case ACTION::Release :
      {
        if(m_State==STATE::Disconnected || m_State==STATE::Initialized|| m_State==STATE::Released) Release();
        else error("Module/Board \"{}\" in state {}. Cannot perform action {}",getIdentifier().getName(),std::string(magic_enum::enum_name(m_State)),action.getActionStr());
        break;
      }
      case ACTION::Quit :
      {
        if(m_State==STATE::Released) Quit();
        else error("Module/Board \"{}\" in state {}. Cannot perform action {}",getIdentifier().getName(),std::string(magic_enum::enum_name(m_State)),action.getActionStr());
        break;
      }
    }
    printAction(action);
  }
}

/*void Module::DoOnData(const Data& data)
{
  printData(data);
}*/
/*
void Module::onOwnMessage(const ix::WebSocketMessagePtr& msg)
{
  Message message;
  message.parse(msg->str);

  if(message.getType() == TYPE::Action) onAction(message);
  else if(message.getType() == TYPE::Open) onOpen(message);
  else if(message.getType() == TYPE::Close) onClose(message);
  else if(message.getType() == TYPE::State) onState(message);
  else if(message.getType() == TYPE::Data) onData(message);
  else if(message.getType() == TYPE::Command) onCommand(message);
  else if(message.getType() == TYPE::Log) onLog(message);
  else if(message.getType() == TYPE::Response) onResponse(message);
  else onUnknown(message);
}*/

void Module::skipConfigFile()
{
  m_UseConfigFile=false;
}

};
