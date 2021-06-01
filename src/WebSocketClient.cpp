#include "WebSocketClient.hpp"

#include "ixwebsocket/IXNetSystem.h"

#include "GeneralParameters.hpp"

namespace yaodaq
{

WebSocketClient::WebSocketClient()
{
  ix::initNetSystem();
}

WebSocketClient::~WebSocketClient()
{
  ix::uninitNetSystem();
}

void WebSocketClient::setUrl(const std::string& url)
{
  ix::WebSocket::setUrl(url);
  m_URLIsSet = true;
}

void WebSocketClient::setHeaderKey(const std::string& key, const std::string& value)
{
  m_Headers[key] = value;
}

void WebSocketClient::start()
{
  if(!m_URLIsSet) ix::WebSocket::setUrl(GeneralParameters::getURL());
  ix::WebSocket::setExtraHeaders(m_Headers);
  ix::WebSocket::start();
}

};
