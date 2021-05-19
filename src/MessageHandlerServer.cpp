#include "MessageHandlerServer.hpp"

#include "ixwebsocket/IXWebSocketMessage.h"
#include "ixwebsocket/IXConnectionState.h"
#include "ixwebsocket/IXWebSocket.h"

#include <iostream>

namespace yaodaq
{
  MessageHandlerServer::MessageHandlerServer()
  {
    m_MessageCallback=[this](std::shared_ptr<ix::ConnectionState> connectionState,ix::WebSocket& webSocket, const ix::WebSocketMessagePtr& msg)
    {
      m_Client = connectionState->getId();
      if(msg->type == ix::WebSocketMessageType::Message)
      {
        this->onMessage(connectionState,webSocket,msg);
      }
      else if(msg->type == ix::WebSocketMessageType::Fragment)
      {
        this->onFragment(connectionState,webSocket,msg);
      }
      else if(msg->type == ix::WebSocketMessageType::Open)
      {
        this->onOpen(connectionState,webSocket,msg);
      }
      else if(msg->type == ix::WebSocketMessageType::Close)
      {
        this->onClose(connectionState,webSocket,msg);
      }
      else if(msg->type == ix::WebSocketMessageType::Error)
      {
        this->onConnectionError(connectionState,webSocket,msg);
      }
      else if(msg->type == ix::WebSocketMessageType::Ping)
      {
        this->onPing(connectionState,webSocket,msg);
      }
      else if(msg->type == ix::WebSocketMessageType::Pong)
      {
        this->onPong(connectionState,webSocket,msg);
      }
    };
  }

  std::string MessageHandlerServer::getClient()
  {
    return m_Client;
  }

  std::function<void(std::shared_ptr<ix::ConnectionState>,ix::WebSocket&, const ix::WebSocketMessagePtr&)> MessageHandlerServer::getMessageCallback()
  {
    return m_MessageCallback;
  }

  void MessageHandlerServer::onMessage(std::shared_ptr<ix::ConnectionState>,ix::WebSocket&, const ix::WebSocketMessagePtr&)
  {
    std::cout<<"message"<<std::endl;
  }

  void MessageHandlerServer::onOpen(std::shared_ptr<ix::ConnectionState>,ix::WebSocket&, const ix::WebSocketMessagePtr&)
  {

  }

  void MessageHandlerServer::onClose(std::shared_ptr<ix::ConnectionState>,ix::WebSocket&, const ix::WebSocketMessagePtr&)
  {
    std::cout<<"close"<<std::endl;
  }

  void MessageHandlerServer::onConnectionError(std::shared_ptr<ix::ConnectionState>,ix::WebSocket&, const ix::WebSocketMessagePtr&)
  {
    std::cout<<"error"<<std::endl;
  }

  void MessageHandlerServer::onPing(std::shared_ptr<ix::ConnectionState>,ix::WebSocket&, const ix::WebSocketMessagePtr&)
  {

  }

  void MessageHandlerServer::onPong(std::shared_ptr<ix::ConnectionState>,ix::WebSocket&, const ix::WebSocketMessagePtr&)
  {

  }

  void MessageHandlerServer::onFragment(std::shared_ptr<ix::ConnectionState>,ix::WebSocket&, const ix::WebSocketMessagePtr&)
  {

  }
};
