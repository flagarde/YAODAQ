#include "MessageHandlerServer.hpp"

#include "ixwebsocket/IXWebSocketMessage.h"
#include "ixwebsocket/IXConnectionState.h"
#include "ixwebsocket/IXWebSocket.h"

#include "Exception.hpp"

#include <iostream>

namespace yaodaq
{
  MessageHandlerServer::MessageHandlerServer(const Identifier& identifier) :  MessageHandler(identifier)
  {
    m_MessageCallback=[this](std::shared_ptr<ix::ConnectionState> connectionState,ix::WebSocket& webSocket, const ix::WebSocketMessagePtr& msg)
    {
      m_Client = connectionState->getId();
      if(msg->type == ix::WebSocketMessageType::Open) onOpen(connectionState,webSocket,msg);
      else if(msg->type == ix::WebSocketMessageType::Close) onClose(connectionState,webSocket,msg);
      else if(msg->type == ix::WebSocketMessageType::Error) onConnectionError(connectionState,webSocket,msg);
      else if(msg->type == ix::WebSocketMessageType::Ping) onPing(connectionState,webSocket,msg);
      else if(msg->type == ix::WebSocketMessageType::Pong) onPong(connectionState,webSocket,msg);
      else if(msg->type == ix::WebSocketMessageType::Message)
      {
        Message m_Message;
        try
        {
          m_Message.parse(msg->str);
        }
        catch(const Exception& exception)
        {
          Log error(LEVEL::Error,m_Clients.getInfos(connectionState->getId()).getIdentifier().get() + " is sending not well formatted messages");
          error.setFrom(getIdentifier().get());
          sendToLogger(error);
        }
        if(m_Message.getType() == TYPE::State)
        {
          printState(m_Message);
          send(m_Message);
        }
        else if(m_Message.getType() == TYPE::Action)
        {
          printAction(m_Message);
          send(m_Message);
        }
        else if(m_Message.getType() == TYPE::Data)
        {
          printData(m_Message);
          send(m_Message);
        }
        else if(m_Message.getType() == TYPE::Log)
        {
          onLog(m_Message);
        }
        else if(m_Message.getType() == TYPE::Command)
        {
          printCommand(m_Message);
          send(m_Message);
          onCommand(m_Message);
        }
        else if(m_Message.getType() == TYPE::Response)
        {
          printResponse(m_Message);
          sendToLogger(m_Message);
          sendToName(m_Message,m_Message.getTo());
        }
        else
        {
          m_Message.setFrom(m_Clients.getInfos(connectionState->getId()).getIdentifier().get());
          logger()->info("Content : {0}; From : {1}; To : {2}", m_Message.getContent(), m_Message.getFrom(), m_Message.getTo());
          send(m_Message);
        }
      }
    };
  }

  void MessageHandlerServer::addFrom(Message& message) const
  {
    if(message.getFrom().empty())
    {
      //If it'a a browser
      if(m_Clients.getInfos(m_Client).isWebBased()) message.setFrom(m_Clients.getInfos(m_Client).getIdentifier().get());
      else  message.setFrom(getIdentifier().get());
    }


  }

  // Send command
  void MessageHandlerServer::send(Message& message)
  {
    addFrom(message);
    for(auto it = m_Clients.begin(); it != m_Clients.end(); ++it)
    {
      if(getClient() != it->first.getID())
      {
        it->second.send(message.get());
      }
    }
  }

  void MessageHandlerServer::sendText(Message& message)
  {
    addFrom(message);
    for(auto it = m_Clients.begin(); it != m_Clients.end(); ++it)
    {
      if(getClient() != it->first.getID()) it->second.sendText(message.get());
    }
  }

  void MessageHandlerServer::sendBinary(Message& message)
  {
    addFrom(message);
    for(auto it = m_Clients.begin(); it != m_Clients.end(); ++it)
    {
      if(getClient() != it->first.getID()) it->second.sendBinary(message.get());
    }
  }

  void MessageHandlerServer::sendToName(Message& message,const std::string& name)
  {
    addFrom(message);
    for(auto it = m_Clients.begin(); it != m_Clients.end(); ++it)
    {
      if(name == it->first.getIdentifier().get()) it->second.sendBinary(message.get());
    }
  }


  void MessageHandlerServer::sendLog(Log& log)
  {
    sendToLogger(log);
    printLog(log);
  }

  void  MessageHandlerServer::sendToLogger(Message& message)
  {
    addFrom(message);
    for(auto it = m_Clients.begin(); it != m_Clients.end(); ++it)
    {
      if( it->first.isA(CLASS::Logger) && getClient() != it->first.getID()) it->second.send(message.get());
    }
  }

  void MessageHandlerServer::sendToLogger(const Message& message)
  {
    for(auto it = m_Clients.begin(); it != m_Clients.end(); ++it)
    {
      if( it->first.isA(CLASS::Logger) && getClient() != it->first.getID()) it->second.send(message.get());
    }
  }


  //If the server receive a log print and send to loggers
  void MessageHandlerServer::onLog(const Log& log)
  {
    sendToLogger(log);
    printLog(log);
  }



  std::string MessageHandlerServer::getClient()
  {
    return m_Client;
  }

  std::function<void(std::shared_ptr<ix::ConnectionState>,ix::WebSocket&, const ix::WebSocketMessagePtr&)> MessageHandlerServer::getMessageCallback()
  {
    return m_MessageCallback;
  }
/*
  void MessageHandlerServer::onMessage(std::shared_ptr<ix::ConnectionState>,ix::WebSocket&, const ix::WebSocketMessagePtr&)
  {
    std::cout<<"message"<<std::endl;
  }
*/
  void MessageHandlerServer::onOpen(std::shared_ptr<ix::ConnectionState> connectionState,ix::WebSocket& webSocket, const ix::WebSocketMessagePtr& msg)
  {
    std::string key = "///";
    std::map<std::string,std::string> headers;
    for(auto it: msg->openInfo.headers)
    {
      headers[it.first]=it.second;
    }

    if(msg->openInfo.headers.find("Key") != msg->openInfo.headers.end())
    {
      key += msg->openInfo.headers["Key"];
    }
    else
    {
      if(msg->openInfo.uri == "/logger") key += "WebLogger/WebLogger/Logger" + std::to_string(m_BrowserNumber);
      else key += "WebBrowser/WebBrowser/Browser" + std::to_string(m_BrowserNumber);
      ++m_BrowserNumber;
    }
    try
    {

      m_Clients.try_emplace(connectionState->getId(), key, webSocket);

    }
    catch(const Exception& exception)
    {
      //Close close(exception.code(),exception.description(),true);
      //close.setFrom(key);
      //printClose(close);
      //sendToAll(close);
      webSocket.stop(exception.code(),exception.description());
      // Registered the not opened connection to knoe their name
      m_NotAccepted[connectionState->getId()]=key;
      return;
    }
    Open open(msg->openInfo.uri,headers,msg->openInfo.protocol);
    std::vector<std::string> all= m_Clients.getClientsIdentifier();
    // all.push_back(getIdentifier().get());
    Json::Value arr =Json::Value(Json::arrayValue);
    for(std::size_t i=0;i!=all.size();++i) arr.append(all[i]);
    open.addKey("All",arr);
    open.setFrom(getIdentifier().get());
    open.addKey("ID", connectionState->getId());
    open.addKey("Key", m_Clients.getInfos(m_Client).getIdentifier().get());

    //counter.add(key);
    printOpen(open);
    send(open);
    sendToName(open,m_Clients.getInfos(m_Client).getIdentifier().get());

  }

  void MessageHandlerServer::onClose(std::shared_ptr<ix::ConnectionState> connectionState,ix::WebSocket& webSocket, const ix::WebSocketMessagePtr& msg)
  {
    Close close(msg->closeInfo.code,msg->closeInfo.reason,msg->closeInfo.remote);
    if(m_NotAccepted.find(connectionState->getId())!=m_NotAccepted.end())
    {
      close.setFrom(m_NotAccepted[connectionState->getId()]);
      m_NotAccepted.erase(connectionState->getId());
      return;
    }
    else close.setFrom(m_Clients.getInfos(m_Client).getKey());

    //
    //std::cout<<"****"<<m_Clients.getInfos(m_Client).getClass()+"/"+m_Clients.getInfos(m_Client).getType()+"/"+m_Clients.getInfos(m_Client).getName()<<std::endl;
    //close.setFrom(m_Clients.getInfos(m_Client).getClass()+"/"+m_Clients.getInfos(m_Client).getType()+"/"+m_Clients.getInfos(m_Client).getName());
    //close.setFrom(m_Clients.getInfos(m_Client).getKey());
    close.addKey("ID", connectionState->getId());
    close.addKey("Key", m_Clients.getInfos(m_Client).getIdentifier().get());
    m_Clients.erase(m_Client);
    std::vector<std::string> all= m_Clients.getClientsIdentifier();
   // all.push_back(getIdentifier().get());
    Json::Value arr =Json::Value(Json::arrayValue);
    for(std::size_t i=0;i!=all.size();++i) arr.append(all[i]);
    close.addKey("All",arr);
    printClose(close);
    send(close);
    //sendToName(close,m_Clients.getInfos(m_Client).getIdentifier().get());

  }

  void MessageHandlerServer::onConnectionError(std::shared_ptr<ix::ConnectionState> connectionState,ix::WebSocket& webSocket, const ix::WebSocketMessagePtr& msg)
  {
    ConnectionError connectionError(msg->errorInfo.retries,msg->errorInfo.wait_time,msg->errorInfo.http_status,msg->errorInfo.reason,msg->errorInfo.decompressionError);
    printConnectionError(connectionError);
  }

  void MessageHandlerServer::onPing(std::shared_ptr<ix::ConnectionState> connectionState,ix::WebSocket& webSocket, const ix::WebSocketMessagePtr& msg)
  {
    Ping ping(msg->str);
    printPing(ping);
    sendToLogger(ping);
  }

  void MessageHandlerServer::onPong(std::shared_ptr<ix::ConnectionState> connectionState,ix::WebSocket& webSocket, const ix::WebSocketMessagePtr& msg)
  {
    Pong pong(msg->str);
    printPong(pong);
    sendToLogger(pong);
  }
/*
  void MessageHandlerServer::onFragment(std::shared_ptr<ix::ConnectionState> connectionState,ix::WebSocket& webSocket, const ix::WebSocketMessagePtr& msg)
  {

  }*/
}
