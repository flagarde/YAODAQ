#include "WebsocketClient.hpp" 
#include "IXNetSystem.h"
#include <iostream>
#include <sstream>
std::string WebsocketClient::m_Url{"ws://localhost:80/"};

WebsocketClient::WebsocketClient(const ix::WebSocketHttpHeaders & header):m_Headers(header)
{
  ix::initNetSystem();
  m_WebSocket.setOnMessageCallback
  (
    [](const ix::WebSocketMessagePtr& msg)
    {
      if (msg->type == ix::WebSocketMessageType::Message)
      {
        std::cout << msg->str << std::endl;
      }
      
      else if (msg->type == ix::WebSocketMessageType::Open)
      {
        std::cout << "send greetings" << std::endl;
        
        // Headers can be inspected (pairs of string/string)
        std::cout << "Handshake Headers:" << std::endl;
        for (auto it : msg->openInfo.headers)
        {
          std::cout << it.first << ": " << it.second << std::endl;
        }
      }
      else if (msg->type == ix::WebSocketMessageType::Close)
      {
        std::cout << "disconnected" << std::endl;
        
        // The server can send an explicit code and reason for closing.
        // This data can be accessed through the closeInfo object.
        std::cout << msg->closeInfo.code << std::endl;
        std::cout << msg->closeInfo.reason << std::endl;
      }
      else if (msg->type == ix::WebSocketMessageType::Error)
      {
        std::stringstream ss;
        ss << "Error: "         << msg->errorInfo.reason      << std::endl;
        ss << "#retries: "      << msg->errorInfo.retries     << std::endl;
        ss << "Wait time(ms): " << msg->errorInfo.wait_time   << std::endl;
        ss << "HTTP Status: "   << msg->errorInfo.http_status << std::endl;
        std::cout << ss.str() << std::endl;
      }
      else if (msg->type == ix::WebSocketMessageType::Ping)
      {
        std::cout << "pong data: " << msg->str << std::endl;
      }
      else if(msg->type == ix::WebSocketMessageType::Pong)
      {
        
      }
    }
  );
  
}

WebsocketClient::~WebsocketClient()
{
  ix::uninitNetSystem();
}

void WebsocketClient::setHeartBeatPeriod(const int& HeartBeat)
{
  m_WebSocket.setHeartBeatPeriod(HeartBeat);
}

void WebsocketClient::start()
{
  m_WebSocket.start();
}

void WebsocketClient::stop()
{
  m_WebSocket.stop();
}

ix::WebSocketSendInfo WebsocketClient::send(const std::string& data,bool binary,const ix::OnProgressCallback& onProgressCallback)
{
  return m_WebSocket.send(data,binary,onProgressCallback);
}

ix::WebSocketSendInfo WebsocketClient::sendBinary(const std::string& text,const ix::OnProgressCallback& onProgressCallback)
{
  return m_WebSocket.sendBinary(text,onProgressCallback);
}

ix::WebSocketSendInfo WebsocketClient::sendText(const std::string& text,const ix::OnProgressCallback& onProgressCallback)
{
  return m_WebSocket.sendText(text,onProgressCallback);
}



