#include "MessageHandler.hpp"

#include "Signals.hpp"
#include "magic_enum.hpp"
#include "spdlog/spdlog.h"

#include <thread>
#include <chrono>

namespace yaodaq
{

  MessageHandler::MessageHandler()
  {
    m_Interrupt.init();
  }

  void MessageHandler::onRaisingSignal()
  {
    static SIGNAL signal;
    do
    {
      signal=m_Interrupt.getSignal();
      std::this_thread::sleep_for(std::chrono::microseconds(5));
    }
    while(signal== yaodaq::SIGNAL::NO);
    int value = magic_enum::enum_integer(signal);
    if(value>=magic_enum::enum_integer(yaodaq::SEVERITY::Critical))
    {
      logger()->critical("Signal {} raised !",SignalName[signal]);
    }
    else if (value>=magic_enum::enum_integer(yaodaq::SEVERITY::Error))
    {
      logger()->error("Signal {} raised !",SignalName[signal]);
    }
    //Should be triggered by user so one character will appears -> Need to return line !
    else if (value>=magic_enum::enum_integer(yaodaq::SEVERITY::Warning))
    {
      fmt::print("\n");
      logger()->warn("Signal {} raised !",SignalName[signal]);
    }
    else if (value>=magic_enum::enum_integer(yaodaq::SEVERITY::Info))
    {
      fmt::print("\n");
      logger()->info("Signal {} raised !",SignalName[signal]);
    }
    else
    {
      fmt::print("\n");
      logger()->trace("Signal {} raised !",SignalName[signal]);
    }
    if(magic_enum::enum_integer(signal)>=magic_enum::enum_integer(SEVERITY::Critical)) std::exit(magic_enum::enum_integer(signal));
  }

  /*MessageHandlerServer::MessageHandlerServer()
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

  }*/
};
