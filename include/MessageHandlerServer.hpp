#pragma once

#include "ixwebsocket/IXWebSocketServer.h"

#include "MessageHandler.hpp"

#include <functional>
#include <memory>
#include <string>

namespace ix
{
  class ConnectionState;
  class WebSocket;
  class WebSocketMessage;
  using WebSocketMessagePtr = std::unique_ptr<WebSocketMessage>;
};

namespace yaodaq
{
  class MessageHandlerServer : public ix::WebSocketServer, public MessageHandler
  {
  public:
    MessageHandlerServer(const int& port, const std::string& host, const int& backlog,const std::size_t& maxConnections, const int& handshakeTimeoutSecs,const int& addressFamily,const Identifier& identifier);
    virtual void onMessage(std::shared_ptr<ix::ConnectionState>,ix::WebSocket&, const ix::WebSocketMessagePtr&);
    virtual void onOpen(std::shared_ptr<ix::ConnectionState>,ix::WebSocket&, const ix::WebSocketMessagePtr&);
    virtual void onClose(std::shared_ptr<ix::ConnectionState>,ix::WebSocket&, const ix::WebSocketMessagePtr&);
    virtual void onConnectionError(std::shared_ptr<ix::ConnectionState>,ix::WebSocket&, const ix::WebSocketMessagePtr&);
    virtual void onPing(std::shared_ptr<ix::ConnectionState>,ix::WebSocket&, const ix::WebSocketMessagePtr&);
    virtual void onPong(std::shared_ptr<ix::ConnectionState>,ix::WebSocket&, const ix::WebSocketMessagePtr&);
    virtual void onFragment(std::shared_ptr<ix::ConnectionState>,ix::WebSocket&, const ix::WebSocketMessagePtr&);
  protected:
    std::string getClient();
    std::function<void(std::shared_ptr<ix::ConnectionState>,ix::WebSocket&, const ix::WebSocketMessagePtr&)> getMessageCallback();
  private:
    std::function<void(std::shared_ptr<ix::ConnectionState>,ix::WebSocket&, const ix::WebSocketMessagePtr&)> m_MessageCallback;
    std::string m_Client;
  };

};
