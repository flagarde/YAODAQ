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

void Module::OnOpen(const std::string&){};
void Module::OnClose(const std::string&){};
void Module::OnPong(const std::string&){};
void Module::OnPing(const std::string&){};
void Module::OnMessage(const std::string&){};
