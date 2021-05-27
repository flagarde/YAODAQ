#include "MessageHandlerClient.hpp"

#include "ixwebsocket/IXWebSocketMessage.h"

#include <iostream>

namespace yaodaq
{
  MessageHandlerClient::MessageHandlerClient(const Identifier& identifier) : MessageHandler(identifier)
  {
    m_MessageCallback=[this](const ix::WebSocketMessagePtr& msg)
    {
      if(msg->type == ix::WebSocketMessageType::Message)
      {
        this->onOwnMessage(msg);
      }
      else if(msg->type == ix::WebSocketMessageType::Fragment)
      {
        this->onOwnFragment(msg);
      }
      else if(msg->type == ix::WebSocketMessageType::Open)
      {
        this->onOwnOpen(msg);
      }
      else if(msg->type == ix::WebSocketMessageType::Close)
      {
        this->onOwnClose(msg);
      }
      else if(msg->type == ix::WebSocketMessageType::Error)
      {
        this->onOwnConnectionError(msg);
      }
      else if(msg->type == ix::WebSocketMessageType::Ping)
      {
        this->onOwnPing(msg);
      }
      else if(msg->type == ix::WebSocketMessageType::Pong)
      {
        this->onOwnPong(msg);
      }
    };
  }

  // Send commands
  void MessageHandlerClient::send(Message& message)
  {
    message.setFrom(getIdentifier().get());
    WebSocketClient::send(message.get());
  }

  void MessageHandlerClient::sendText(Message& message)
  {
    message.setFrom(getIdentifier().get());
    WebSocketClient::sendText(message.get());
  }

  void MessageHandlerClient::sendBinary(Message& message)
  {
    message.setFrom(getIdentifier().get());
    WebSocketClient::sendBinary(message.get());
  }









  std::function<void(const ix::WebSocketMessagePtr&)> MessageHandlerClient::getMessageCallback()
  {
    return m_MessageCallback;
  }

  void MessageHandlerClient::onOwnMessage(const ix::WebSocketMessagePtr& msg)
  {
    std::cout<<"message"<<std::endl;
  }

  void MessageHandlerClient::onOwnOpen(const ix::WebSocketMessagePtr& msg)
  {

  }

  void MessageHandlerClient::onOwnClose(const ix::WebSocketMessagePtr& msg)
  {
    std::cout<<"close"<<std::endl;
  }

  void MessageHandlerClient::onOwnConnectionError(const ix::WebSocketMessagePtr& msg)
  {
    std::cout<<"error"<<std::endl;
  }

  void MessageHandlerClient::onOwnPing(const ix::WebSocketMessagePtr& msg)
  {

  }

  void MessageHandlerClient::onOwnPong(const ix::WebSocketMessagePtr& msg)
  {

  }

  void MessageHandlerClient::onOwnFragment(const ix::WebSocketMessagePtr& msg)
  {

  }
};
