#include "Module.hpp"

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

Module::~Module()
{
  m_WebsocketClient.stop();
}


void Module::Initialize(){};
void Module::Connect(){};
void Module::Configure(){};
void Module::Start(){};
void Module::Pause(){};
void Module::Stop(){};
void Module::Clear(){};
void Module::Disconnect(){};
void Module::Release(){};

void Module::DoOnMessage(const ix::WebSocketMessagePtr& msg)
{
  
  std::cout << msg->str << std::endl;
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
