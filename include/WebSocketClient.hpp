#pragma once

#include "GeneralParameters.hpp"

#include "ixwebsocket/IXWebSocket.h"

namespace yaodaq
{

  class WebSocketClient: public ix::WebSocket
  {
  public:
    WebSocketClient();
    ~WebSocketClient();
    void setUrl(const std::string& url);
    void start();
    void setHeaderKey(const std::string& key, const std::string& value);

  private:
    ix::WebSocketHttpHeaders m_Headers;
    bool                     m_URLIsSet{false};
  };

};
