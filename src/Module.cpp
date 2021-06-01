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
  setHeaderKey("Key", getIdentifier().get());
  setOnMessageCallback(getMessageCallback());
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
  stop();
  while(getReadyState()!=ix::ReadyState::Closed) std::this_thread::sleep_for(std::chrono::microseconds(10));
}

void Module::startListening()
{
  logger()->info("Listening on {}.",getUrl());
  start();
  while(getReadyState()!=ix::ReadyState::Open) std::this_thread::sleep_for(std::chrono::microseconds(10));
}

void Module::setState(const States& state)
{
  m_State = state;
}

States Module::getState()
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

/*void Module::sendState()
{
  State state(m_State);
  MessageHandlerClient::sendBinary(state);
  logger()->warn(state.get());
}*/

void Module::Initialize()
{
  try
  {
    if(m_UseConfigFile)LoadConfig();
    DoInitialize();
    sendState(States::INITIALIZED);
    setState(States::INITIALIZED);
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
    sendState(States::CONNECTED);
    setState(States::CONNECTED);
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
    sendState(States::CONFIGURED);
    setState(States::CONFIGURED);
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
    sendState(States::STARTED);
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
    setState(States::PAUSED);
    if(m_LoopOnStartUsed == true)
    {
      m_LoopOnStart.join();
      m_LoopOnStartUsed = false;
    }
    DoPause();
    sendState(States::PAUSED);
  }
  catch(const Exception& exception)
  {
    error(exception.what());
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
    sendState(States::STOPPED);
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
    setState(States::CLEARED);
    sendState(States::CLEARED);
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
    setState(States::DISCONNECTED);
    sendState(States::DISCONNECTED);
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
    setState(States::RELEASED);
    sendState(States::RELEASED);
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
    setState(States::QUITED);
    sendState(States::QUITED);
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
  if(m_command.getCommand() == "getState") sendState(m_State);
}

void Module::DoOnAction(const Action& action)
{
  if(getIdentifier().getClass() != CLASS::Logger && getIdentifier().getClass() != CLASS::Controller)
  {
    auto action_ = magic_enum::enum_cast<Actions>(action.getContent());
    if(action_.has_value())
    {
      if((m_State==States::UNINITIALIZED || m_State==States::INITIALIZED|| m_State==States::RELEASED) && action_.value()== Actions::INITIALIZE) Initialize();
      else if((m_State==States::INITIALIZED || m_State==States::DISCONNECTED||m_State==States::CONNECTED) && action_.value()== Actions::CONNECT ) Connect();
      else if((m_State==States::CONNECTED || m_State==States::CLEARED || m_State==States::CONFIGURED ) && action_.value()== Actions::CONFIGURE ) Configure();
      else if((m_State==States::CONFIGURED || m_State==States::STOPPED || m_State==States::STARTED || m_State==States::PAUSED) && action_.value()== Actions::START ) Start();
      else if((m_State==States::STARTED || m_State==States::PAUSED) && action_.value()== Actions::PAUSE) Pause();
      else if((m_State==States::STARTED || m_State==States::PAUSED || m_State==States::STOPPED)  && action_.value()== Actions::STOP) Stop();
      else if((m_State==States::STOPPED || m_State==States::CONFIGURED || m_State==States::CLEARED) && action_.value()== Actions::CLEAR) Clear();
      else if((m_State==States::CLEARED || m_State==States::CONNECTED|| m_State==States::DISCONNECTED) && action_.value()== Actions::DISCONNECT) Disconnect();
      else if((m_State==States::DISCONNECTED || m_State==States::INITIALIZED|| m_State==States::RELEASED) && action_.value()== Actions::RELEASE) Release();
      else if(m_State==States::RELEASED && action_.value()== Actions::QUIT) Quit();
      else error("Module/Board \"{}\" in state {}. Cannot perform action {}",getIdentifier().getName(),std::string(magic_enum::enum_name(m_State)),std::string(magic_enum::enum_name(action_.value())));
    }
    printAction(action);
  }
}

/*void Module::DoOnData(const Data& data)
{
  printData(data);
}*/

void Module::onOwnMessage(const ix::WebSocketMessagePtr& msg)
{
  Message message;
  message.parse(msg->str);

  if(message.getType() == TYPE::Action) DoOnAction(message);
  else if(message.getType() == TYPE::Open) onOpen(message);
  else if(message.getType() == TYPE::Close) onClose(message);
  else if(message.getType() == TYPE::State) onState(message);
  else if(message.getType() == TYPE::Data) onData(message);
  else if(message.getType() == TYPE::Command) DoOnCommand(message);
  else if(message.getType() == TYPE::Log) onLog(message);
  else OnMessage(msg);
}

void Module::skipConfigFile()
{
  m_UseConfigFile=false;
}

void Module::OnMessage(const ix::WebSocketMessagePtr& msg)
{
  logger()->info("{}", msg->str);
}

};
