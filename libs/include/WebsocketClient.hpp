#pragma once

#include "IXWebSocket.h"
#include "GeneralParameters.hpp"

class WebsocketClient : public ix::WebSocket
{
public:
  WebsocketClient();
  ~WebsocketClient();
  void setUrl(const std::string& url);
  void                  start();
  void                  setHeaderKey(const std::string& key, const std::string& value);

private:
  ix::WebSocketHttpHeaders m_Headers;
  bool                     m_URLIsSet{false};
};
