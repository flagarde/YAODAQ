#include "Module.hpp"

std::string Module::getName()
{
  return m_Name;
}
std::string Module::getType()
{
  return m_Type;
}

Module::Module(const std::string& name, const std::string& type):m_Name(name),m_Type(type)
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
