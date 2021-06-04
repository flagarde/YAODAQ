#include "MessageHandlerClient.hpp"

#include "ixwebsocket/IXWebSocketMessage.h"

#include "StatusCode.hpp"

#include <iostream>

namespace yaodaq
{
  MessageHandlerClient::MessageHandlerClient(const Identifier& identifier) : MessageHandler(identifier)
  {
    m_MessageCallback=[this](const ix::WebSocketMessagePtr& msg)
    {
      if(msg->type == ix::WebSocketMessageType::Message)
      {
        onOwnMessage(msg);
      }
      else if(msg->type == ix::WebSocketMessageType::Fragment)
      {
        onOwnFragment(msg);
      }
      else if(msg->type == ix::WebSocketMessageType::Open)
      {
        onOwnOpen(msg);
      }
      else if(msg->type == ix::WebSocketMessageType::Close)
      {
        onOwnClose(msg);
      }
      else if(msg->type == ix::WebSocketMessageType::Error)
      {
        onOwnConnectionError(msg);
      }
      else if(msg->type == ix::WebSocketMessageType::Ping)
      {
        onOwnPing(msg);
      }
      else if(msg->type == ix::WebSocketMessageType::Pong)
      {
        onOwnPong(msg);
      }
    };
  }

  void MessageHandlerClient::sendData(Data& data)
  {
    sendBinary(data);
    printData(data);
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

  //
  void MessageHandlerClient::sendLog(Log& log)
  {
    sendText(log);
    printLog(log);
  }

  void MessageHandlerClient::sendState(const STATE& state)
  {
    State states(state);
    sendText(states);
    printState(states);
  }

  void MessageHandlerClient::onAction(const Action& action)
  {

  }

  // Open
  void MessageHandlerClient::onOpen(const Open& open)
  {
    if(getIdentifier().getClass()==CLASS::Logger) printOpen(open);
  }

  void MessageHandlerClient::onOwnOpen(const ix::WebSocketMessagePtr& message)
  {
    std::string headers;
    for(auto it: message->openInfo.headers)
    {
      headers+=fmt::format("\t{} : {}\n", it.first,it.second);
    }
    logger()->info(fmt::format(fg(fmt::color::green) | fmt::emphasis::bold,"Connected :\nUri : {}\nHeaders :\n{}Protocol : {}\n",message->openInfo.uri,headers,message->openInfo.protocol));
  }

  // Close

  void MessageHandlerClient::sendPing(const std::string& pin)
  {
    ping(pin);
  }



  void MessageHandlerClient::onOwnClose(const ix::WebSocketMessagePtr& msg)
  {
    Close close(msg->closeInfo.code,msg->closeInfo.reason,msg->closeInfo.remote);
    printClose(close);
    if(msg->closeInfo.code== static_cast<int16_t>(StatusCode::ALREADY_PRESENT))
    {
      disableAutomaticReconnection();
      std:exit(static_cast<int16_t>(StatusCode::ALREADY_PRESENT));
    }
  }

  void MessageHandlerClient::onOwnConnectionError(const ix::WebSocketMessagePtr& msg)
  {
    ConnectionError connectionError(msg->errorInfo.retries,msg->errorInfo.wait_time,msg->errorInfo.http_status,msg->errorInfo.reason,msg->errorInfo.decompressionError);
    printConnectionError(connectionError);
  }

  void MessageHandlerClient::onOwnPing(const ix::WebSocketMessagePtr& msg)
  {
    Ping ping(msg->str);
    printPing(ping);
  }

  void MessageHandlerClient::onOwnPong(const ix::WebSocketMessagePtr& msg)
  {
    Pong pong(msg->str);
    printPong(pong);
  }


  std::function<void(const ix::WebSocketMessagePtr&)> MessageHandlerClient::getMessageCallback()
  {
    return m_MessageCallback;
  }

  void MessageHandlerClient::onOwnMessage(const ix::WebSocketMessagePtr& msg)
  {
  }

  void MessageHandlerClient::onOwnFragment(const ix::WebSocketMessagePtr& msg)
  {

  }
};
