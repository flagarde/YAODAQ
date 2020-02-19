#include "WebsocketServer.hpp" 
#include "IXNetSystem.h"
#include "Message.hpp"
#include "spdlog.h"
#include "Error.hpp"

#include <iostream>

WebsocketServer::WebsocketServer(const int& port,const std::string& host,const int& backlog,const std::size_t& maxConnections,const int& handshakeTimeoutSecs):m_Server(port,host,backlog,maxConnections,handshakeTimeoutSecs)
{
  ix::initNetSystem();
  m_Server.setOnConnectionCallback
  (
    [this](std::shared_ptr<ix::WebSocket> webSocket,std::shared_ptr<ix::ConnectionState> connectionState) 
    {
      webSocket->setOnMessageCallback
      (
        [webSocket, connectionState,this](const ix::WebSocketMessagePtr& msg) 
        {
          static int browserNumber=1;
          if (msg->type == ix::WebSocketMessageType::Open)
          {
            std::string key="///";
            //Print in webserver console
            spdlog::info("New connection ID : {}",connectionState->getId());
            spdlog::info("Uri : {}",msg->openInfo.uri);
            spdlog::info("Headers :");
            for (auto it : msg->openInfo.headers)
            {
              spdlog::info("\t{} : {}",it.first,it.second);
            }
            //
            if(msg->openInfo.headers.find("Key")!=msg->openInfo.headers.end())
            {
              key=msg->openInfo.headers["Key"];
            }
            else
            {
              key+="Browser/Browser"+std::to_string(browserNumber);
              ++browserNumber;
            }
            
            //Error coming from Error here should not crash the Webserver but disconnect the websocket of the bad guy !"
            //Look if the name is already taken an put an erro if it is
            Infos infos;
            try
            {
              infos.setKey(key);
            }
            catch(const Error& err)
            {
              spdlog::error(err.what());
              Message error("Error",err.what(),"","WebSocketServer");
              for(std::map<Infos,std::shared_ptr<ix::WebSocket>>::iterator it=m_Clients.begin();it!=m_Clients.end();++it)
              {
                if(it->first.getType()=="Logger") 
                {
                  it->second->send(error.get());
                }
              }
              webSocket->send(error.get());
              std::this_thread::sleep_for(std::chrono::milliseconds(1));
              if(webSocket->getReadyState()!=ix::ReadyState::Closing&&webSocket->getReadyState()!=ix::ReadyState::Closed) webSocket->stop(1002,"Exit");
            }
            if(m_Clients.find(infos)!=m_Clients.end())
            {
              spdlog::error("The Name \"{}\" is already taken !\n Disconnecting !",infos.getName());
              Message error("Error","The Name \""+infos.getName()+"\" is already taken !\n Disconnecting !","","WebSocketServer");
              for(std::map<Infos,std::shared_ptr<ix::WebSocket>>::iterator it=m_Clients.begin();it!=m_Clients.end();++it)
              {
                std::cout<<"********"<<it->first.getName()<<std::endl;
                if(it->first.getType()=="Logger") 
                {
                  //it->second->send(error.get());
                }
              }
              webSocket->send(error.get());
              std::this_thread::sleep_for(std::chrono::milliseconds(1));
              if(webSocket->getReadyState()!=ix::ReadyState::Closing&&webSocket->getReadyState()!=ix::ReadyState::Closed) webSocket->stop(1002,"Exit");
              
            }
            else m_Clients.emplace(infos,webSocket);
            //Print in all the Logger consoles
            for(std::map<Infos,std::shared_ptr<ix::WebSocket>>::iterator it=m_Clients.begin();it!=m_Clients.end();++it)
            {
              if(it->first.getType()=="Logger") 
              {
                it->second->send("New connection ID : "+connectionState->getId());
                it->second->send("Uri :  : "+msg->openInfo.uri);
                it->second->send("Headers :");
                for (auto itt : msg->openInfo.headers)
                {
                  //it->second->send("\t"+itt.first+" : "+itt.second);
                }
              }
            }
          }
          else if(msg->type == ix::WebSocketMessageType::Close)
          {
            spdlog::info("Closed connection ID : {}",connectionState->getId());
            //
            for(std::map<Infos,std::shared_ptr<ix::WebSocket>>::iterator it=m_Clients.begin();it!=m_Clients.end();++it)
            {
              if(it->first.getType()=="Logger"&&webSocket!=it->second) 
              {
                it->second->send("Closed connection ID : "+connectionState->getId());
              }
            }
            erase(webSocket);
            std::cout<<"After cLOSINNGGGG"<<std::endl;
            for(std::map<Infos,std::shared_ptr<ix::WebSocket>>::iterator it=m_Clients.begin();it!=m_Clients.end();++it)
            {
              std::cout<<"After Clossing "<<it->first.getName()<<std::endl;
            }
          }
          else if(msg->type == ix::WebSocketMessageType::Message)
          {
            spdlog::info("Message :");
            spdlog::info("\t{}",msg->str);
            for(std::map<Infos,std::shared_ptr<ix::WebSocket>>::iterator it=m_Clients.begin();it!=m_Clients.end();++it)
            {
              if(it->second!=webSocket)it->second->send(msg->str);
            }
          }
        }
      );
    }
  );  
}

void WebsocketServer::erase(const std::shared_ptr<ix::WebSocket>& socket)
{
  
  for(std::map<Infos,std::shared_ptr<ix::WebSocket>>::iterator it=m_Clients.begin();it!=m_Clients.end();++it)
  {
    if(it->second==socket)
    {
      m_Clients.erase(it);
      break;
    }
  }
}

WebsocketServer::~WebsocketServer()
{
  stop();
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

