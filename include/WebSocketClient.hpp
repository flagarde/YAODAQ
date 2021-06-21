#pragma once

#include "ixwebsocket/IXWebSocket.h"

namespace yaodaq
{

class WebSocketClient: public ix::WebSocket
{
public:
  WebSocketClient();
  ~WebSocketClient();
  void setUrl(const std::string&);
  void start();
  void setHeaderKey(const std::string&, const std::string&);

private:
  ix::WebSocketHttpHeaders m_Headers;
  bool                     m_URLIsSet{false};
};

}
