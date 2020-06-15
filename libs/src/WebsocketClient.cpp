#include "WebsocketClient.hpp"
#include "IXNetSystem.h"

WebsocketClient::WebsocketClient()
{
  ix::initNetSystem();
}

WebsocketClient::~WebsocketClient()
{
  ix::uninitNetSystem();
}

void WebsocketClient::setUrl(const std::string& url)
{
  ix::WebSocket::setUrl(url);
  m_URLIsSet=true;
}


void WebsocketClient::setHeaderKey(const std::string& key, const std::string& value)
{
  m_Headers[key] = value;
}



void WebsocketClient::start()
{
  if(!m_URLIsSet) ix::WebSocket::setUrl(GeneralParameters::getURL());
  ix::WebSocket::setExtraHeaders(m_Headers);
  ix::WebSocket::start();
}
