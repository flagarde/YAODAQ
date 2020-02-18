#include "WebsocketServer.hpp" 
#include "IXNetSystem.h"
#include "ConnectionState.hpp"
#include "Log.hpp"
#include "spdlog.h"

#include <iostream>

WebsocketServer::WebsocketServer(const int& port,const std::string& host,const int& backlog,const std::size_t& maxConnections,const int& handshakeTimeoutSecs):m_Server(port,host,backlog,maxConnections,handshakeTimeoutSecs)
{
  ix::initNetSystem();
  m_Server.setConnectionStateFactory(&ConnectionState::createConnectionState);
  m_Server.setOnConnectionCallback
  (
    [this](std::shared_ptr<ix::WebSocket> webSocket,std::shared_ptr<ix::ConnectionState> connectionState) 
    {
      webSocket->setOnMessageCallback
      (
        [webSocket, connectionState,this](const ix::WebSocketMessagePtr& msg) 
        {
          static int browserNumber=1;
          std::shared_ptr<ConnectionState> state =std::static_pointer_cast<ConnectionState>(connectionState);
          if (msg->type == ix::WebSocketMessageType::Open)
          {
            spdlog::info("New connection ID : {}",connectionState->getId());
            spdlog::info("Uri : {}",msg->openInfo.uri);
            spdlog::info("Headers :");
            for (auto it : msg->openInfo.headers)
            {
              spdlog::info("\t{} : {}",it.first,it.second);
            }
            if(msg->openInfo.headers.find("Name")!=msg->openInfo.headers.end()) state->setName(msg->openInfo.headers["Name"]);
            else
            {
              state->setName("Browser"+std::to_string(browserNumber));
              ++browserNumber;
            }
            if(msg->openInfo.headers.find("Type")!=msg->openInfo.headers.end()) state->setType(msg->openInfo.headers["Type"]);
            else state->setType("Browser");
          }
          else if (msg->type == ix::WebSocketMessageType::Close)
          {
            spdlog::info("Closed connection ID : {}",connectionState->getId());
          }
          else if (msg->type == ix::WebSocketMessageType::Message)
          {
            spdlog::info("Message :\n{}",msg->str);
            for (auto&& client : m_Server.getClients())
            {
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

void WebsocketServer::start()
{
  m_Server.start();
}

void WebsocketServer::stop()
{
  m_Server.stop();
}

void WebsocketServer::wait()
{
  m_Server.wait();
}

void  WebsocketServer::listen()
{
  std::pair<bool, std::string> res = m_Server.listen();
  if(!res.first)
  {
    spdlog::error("{}",res.second);
    std::exit(1);
  } 
}

