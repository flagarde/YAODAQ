#include "Module.hpp"
#include "parser.hpp"
#include "literal.hpp"
#include "serializer.hpp"
#include "get.hpp"

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

Module::Module( const std::string& type,const std::string& name):m_Name(name),m_Type(type)
{
  m_WebsocketClient.setExtraHeader("Name",m_Name);
  m_WebsocketClient.setExtraHeader("Type",m_Type);
  m_WebsocketClient.setOnMessageCallback(m_CallBack);
  m_WebsocketClient.start();
  
}

void Module::DoDoConnect()
{
  
}

void Module::DoDoDisconnect()
{
  
}

void Module::LoadConfig()
{
  m_Config.parse();
  m_Conf=m_Config.getConfig(m_Name);
}


void Module::printParameters()
{
  std::cout<<m_Conf<<std::endl;
}

Module::~Module()
{
  m_WebsocketClient.stop();
}

void Module::Initialize()
{
  LoadConfig();
  DoInitialize();
  sendStatus("INITIALIZED");
}

void Module::Connect()
{
  DoDoConnect();
  sendStatus("CONNECTED");
}

void Module::Configure()
{
  DoConfigure();
  sendStatus("CONFIGURE");
}

void Module::Start()
{
  DoStart();
  sendStatus("STARTED");
}

void Module::Pause()
{
  DoPause();
  sendStatus("PAUSED");
}

void Module::Stop()
{
  DoStop();
  sendStatus("STOPED");
}

void Module::Clear()
{
  DoClear();
  sendStatus("CLEARED");
}

void Module::Disconnect()
{
  DoDoDisconnect();
  sendStatus("DISCONNECTED");
}

void Module::Release()
{
  DoRelease();
  sendStatus("RELEASED");
}

void Module::Quit()
{
  DoQuit();
  sendStatus("QUITED");
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
  std::stringstream ss;
  ss << "Error: "         << msg->errorInfo.reason      << std::endl;
  ss << "#retries: "      << msg->errorInfo.retries     << std::endl;
  ss << "Wait time(ms): " << msg->errorInfo.wait_time   << std::endl;
  ss << "HTTP Status: "   << msg->errorInfo.http_status << std::endl;
  std::cout << ss.str() << std::endl;
}
