#include "Module.hpp"
#include "spdlog.h"
#include "sinks/stdout_color_sinks.h"
#include "sinks/ostream_sink.h"
#include "sinks/ansicolor_sink.h"
#include "toml.hpp"
#include "LoggerSink.hpp"

Configuration Module::m_Config=Configuration();

void  Module::setConfigFile(const std::string& file)
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
  message.setFrom(m_Type+"/"+m_Name);
  return m_WebsocketClient.sendBinary(message.get());
}

ix::WebSocketSendInfo Module::sendText(Message& message)
{
  message.setFrom(m_Type+"/"+m_Name);
  return m_WebsocketClient.sendText(message.get());
}

Module::Module( const std::string& name,const std::string& type):m_Type(type),m_Name(name)
{
  std::cout<<"Creating Module"<<std::endl;
  std::cout<<name<<"  "<<type<<std::endl;
  m_WebsocketClient.setExtraHeader("Key","///"+m_Type+"/"+m_Name);
  m_WebsocketClient.setOnMessageCallback(m_CallBack);
  m_WebsocketClient.start();
  std::shared_ptr<spdlog::sinks::sink> console = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
  std::shared_ptr<spdlog::sinks::sink> websocketlogger = std::make_shared<WebSocketLoguer_mt>(m_WebsocketClient,m_Type+"/"+m_Name);
  spdlog::sinks_init_list sink_list={console,websocketlogger};
  m_Logger= std::make_shared<spdlog::logger>(m_Type+"/"+m_Name,sink_list.begin(),sink_list.end());
  std::cout<<"Finished Creating Module"<<std::endl;
}

void Module::DoDoConnect()
{
  
}

void Module::DoDoDisconnect()
{
  
}

void Module::verifyParameters()
{
  std::cout<<"Module VerifyParameters"<<std::endl;
}

void Module::LoadConfig()
{
    m_Config.parse();
    m_Conf=m_Config.getConfig(m_Name);
    this->verifyParameters();
}


void Module::printParameters()
{
  std::cout<<m_Conf<<std::endl;
}

Module::~Module()
{
  std::cout<<"Destroying Module"<<std::endl;
  m_WebsocketClient.stop();
  std::cout<<"Finished Destroying Module"<<std::endl;
  
}

void Module::Initialize()
{
  try
  {
    LoadConfig();
    DoInitialize();
    sendStatus("INITIALIZED");
  }
  catch(const std::exception& error)
  {
    m_Logger->error(error.what());
    throw error;
  }
}

void Module::Connect()
{
  try
  {
    DoDoConnect();
    sendStatus("CONNECTED");
  }
  catch(const std::exception& error)
  {
    m_Logger->error(error.what());
    throw error;
  }
}

void Module::Configure()
{
  try
  {
    DoConfigure();
    sendStatus("CONFIGURE");
  }
  catch(const std::exception& error)
  {
    m_Logger->error(error.what());
    throw error;
  }
}

void Module::Start()
{
  try
  {
    DoStart();
    sendStatus("STARTED");
  }
  catch(const std::exception& error)
  {
    m_Logger->error(error.what());
    throw error;
  }
}

void Module::Pause()
{
  try
  {
    DoPause();
    sendStatus("PAUSED");
  }
  catch(const std::exception& error)
  {
    m_Logger->error(error.what());
    throw error;
  }
}

void Module::Stop()
{
  try
  {
    DoStop();
    sendStatus("STOPED");
  }
  catch(const std::exception& error)
  {
    m_Logger->error(error.what());
    throw error;
  }
}

void Module::Clear()
{
  try
  {
    DoClear();
    sendStatus("CLEARED");
  }
  catch(const std::exception& error)
  {
    m_Logger->error(error.what());
    throw error; 
  }
}

void Module::Disconnect()
{
  try
  {
    DoDoDisconnect();
    sendStatus("DISCONNECTED");
  }
  catch(const std::exception& error)
  {
    m_Logger->error(error.what());
    throw error;
  }
}

void Module::Release()
{
  try
  {
    DoRelease();
    sendStatus("RELEASED");
  }
  catch(const std::exception& error)
  {
    m_Logger->error(error.what());
    throw error;
  }
}

void Module::Quit()
{
  try
  {
    DoQuit();
    sendStatus("QUITED");
  }
  catch(const std::exception& error)
  {
    m_Logger->error(error.what());
    throw error;
  }
}

void Module::sendStatus(const std::string& stat)
{
  Status status;
  status.setContent(stat);
  sendText(status);
}


void Module::DoInitialize()
{
  std::cout<<"Initialize"<<std::endl;
};

void Module::DoConnect()
{
  std::cout<<"Connect"<<std::endl;
};

void Module::DoConfigure()
{
  std::cout<<"Configure"<<std::endl;
};

void Module::DoStart()
{
  std::cout<<"Start"<<std::endl;
};

void Module::DoPause()
{
  std::cout<<"Pause"<<std::endl;
};

void Module::DoStop()
{
  std::cout<<"Stop"<<std::endl;
};

void Module::DoClear()
{
  std::cout<<"Clear"<<std::endl;
};

void Module::DoDisconnect()
{
  std::cout<<"Disconnect"<<std::endl;
};

void Module::DoRelease()
{
  std::cout<<"Release"<<std::endl;
};

void Module::DoQuit()
{
  std::cout<<"Quit"<<std::endl;
}

void Module::DoOnStatus(Message& message)
{
  if(message.getContent()=="INITIALIZE")
  {
    Initialize();
  }
  else if(message.getContent()=="CONNECT")
  {
    Connect();
  }
  else if(message.getContent()=="CONFIGURE")
  {
    Configure();
  }
  else if(message.getContent()=="START")
  {
    Start();
  }
  else if(message.getContent()=="PAUSE") 
  {
    Pause();
  }
  else if(message.getContent()=="STOP") 
  {
    Stop();
  }
  else if(message.getContent()=="CLEAR")
  {
    Clear();
  }
  else if(message.getContent()=="DISCONNECT")
  {
    Disconnect();
  }
  else if(message.getContent()=="RELEASE")
  {
    Release();
  }
  else if(message.getContent()=="QUIT")
  {
    Quit();
  }
  else if(message.getContent()=="UNKNOWN_STATUS")
  {
    std::cout<<"You type an UNKNOWN_STATUS "<<std::endl;
  }
}

void Module::DoOnMessage(const ix::WebSocketMessagePtr& msg)
{
  Message message;
  message.parse(msg->str);
  message.print();
  if(message.getType()=="Status")
  {
    DoOnStatus(message);
  }
  else OnMessage(msg);
}



void Module::OnOpen(const ix::WebSocketMessagePtr& msg)
{
  // Headers can be inspected (pairs of string/string)
  std::cout << "Handshake Headers :" << std::endl;
  for (auto it : msg->openInfo.headers)
  {
    std::cout << it.first << ": " << it.second << std::endl;
  }
}

void Module::OnClose(const ix::WebSocketMessagePtr& msg)
{
  std::cout << "Disconnected !" << std::endl;
  
  // The server can send an explicit code and reason for closing.
  // This data can be accessed through the closeInfo object.
  std::cout << msg->closeInfo.code << std::endl;
  std::cout << msg->closeInfo.reason << std::endl;
}

void Module::OnPong(const ix::WebSocketMessagePtr& msg)
{
  std::cout << "Pong data: " << msg->str << std::endl;
}

void Module::OnPing(const ix::WebSocketMessagePtr& msg)
{
  std::cout << "Ping data: " << msg->str << std::endl;
}

void Module::OnMessage(const ix::WebSocketMessagePtr& msg)
{
  std::cout << msg->str << std::endl;
}

void Module::OnError(const ix::WebSocketMessagePtr& msg)
{
  // The server can send an explicit code and reason for closing.
  // This data can be accessed through the closeInfo object.
  spdlog::info("Disconnected !");
  spdlog::info("{}",msg->closeInfo.code);
  spdlog::info("{}",msg->closeInfo.reason); 
  if(msg->closeInfo.code==1002) throw Error(msg->closeInfo.code,msg->closeInfo.reason);
}
