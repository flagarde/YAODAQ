#pragma once

#include <string>
#include <functional>

#include "IXWebSocket.h"
#include "IXWebSocketMessage.h"

class WebsocketClient 
{
public:
  WebsocketClient(const ix::WebSocketHttpHeaders & header=ix::WebSocketHttpHeaders());
  ~WebsocketClient();
  void setHeartBeatPeriod(const int& HeartBeat);
  void start();
  void stop();
  ix::ReadyState getReadyState();
  void disableAutomaticReconnection();
  void setOnMessageCallback(std::function<void(const std::shared_ptr<ix::WebSocketMessage>)> m_func);
  ix::WebSocketSendInfo send(const std::string& data,bool binary = false,const ix::OnProgressCallback& onProgressCallback = nullptr);
  ix::WebSocketSendInfo sendBinary(const std::string& text,const ix::OnProgressCallback& onProgressCallback = nullptr);
  ix::WebSocketSendInfo sendText(const std::string& text,const ix::OnProgressCallback& onProgressCallback = nullptr);
  void setExtraHeader(const std::string& key,const std::string& value);
  static void setURL(const std::string& url);
private:
  static std::string m_Url;
  ix::WebSocket m_WebSocket;
  ix::WebSocketHttpHeaders m_Headers;
  std::function<void(const std::shared_ptr<ix::WebSocketMessage>)> m_SocketMessagePtr{nullptr};
};
