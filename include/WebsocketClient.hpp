#pragma once

#include "GeneralParameters.hpp"
#include "IXWebSocket.h"

class WebsocketClient: public ix::WebSocket
{
public:
  WebsocketClient();
  ~WebsocketClient();
  void setUrl(const std::string& url);
  void start();
  void setHeaderKey(const std::string& key, const std::string& value);

private:
  ix::WebSocketHttpHeaders m_Headers;
  bool                     m_URLIsSet{false};
};
