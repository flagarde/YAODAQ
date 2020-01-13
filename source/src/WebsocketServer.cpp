#include "WebsocketServer.hpp" 
#include "IXNetSystem.h"
#include "ConnectionState.hpp"
#include <iostream>

WebsocketServer::WebsocketServer(const int& port,const std::string& host,const int& backlog,const std::size_t& maxConnections):m_Server(port,host,backlog,maxConnections)
{
  ix::initNetSystem();
  // Test that we can override the connectionState impl to provide our own
 /* auto factory = []() -> std::shared_ptr<ix::ConnectionState> 
  {
    return std::make_shared<ConnectionStateCustom>();
  };*/
  m_Server.setConnectionStateFactory(&ConnectionState::createConnectionState);
  
  m_Server.setOnConnectionCallback
  (
    [this](std::shared_ptr<ix::WebSocket> webSocket,std::shared_ptr<ix::ConnectionState> connectionState) 
    {
      webSocket->setOnMessageCallback
      (
        [webSocket, connectionState,this](const ix::WebSocketMessagePtr& msg) 
        {
          std::shared_ptr<ConnectionState> state =std::static_pointer_cast<ConnectionState>(connectionState);
          if (msg->type == ix::WebSocketMessageType::Open)
          {
            std::cout << "New connection id: " << connectionState->getId()<<std::endl;
            std::cout << "Uri: " << msg->openInfo.uri<<std::endl;
            std::cout << "Headers:"<<std::endl;
            for (auto it : msg->openInfo.headers)
            {
              std::cout <<"\t"<< it.first << ": " << it.second<<std::endl;
            }
            std::cout<< "Type : "<<state->getType()<<std::endl;
          }
          else if (msg->type == ix::WebSocketMessageType::Close)
          {
            std::cout << "Closed connection id : "<<connectionState->getId() <<std::endl;;
          }
          else if (msg->type == ix::WebSocketMessageType::Message)
          {
            for (auto&& client : m_Server.getClients())
            {
              std::cout<<msg->str<<std::endl;
              if (client != webSocket)
              {
                client->send(msg->str);
              }
            }
          }
        }
      );
    }
  );  
}

WebsocketServer::~WebsocketServer()
{
  ix::uninitNetSystem();
}

void WebsocketServer::Start()
{
  m_Server.start();
}

void WebsocketServer::Stop()
{
  m_Server.stop();
}

void WebsocketServer::Wait()
{
  m_Server.wait();
}

void  WebsocketServer::Listen()
{
  std::pair<bool, std::string> res = m_Server.listen();
  if(!res.first)
  {
    std::cout<<res.second<<std::endl;
    std::exit(1);
  } 
}

