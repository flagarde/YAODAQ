#ifndef WEBSOCKETCLIENT_HPP
#define WEBSOCKETCLIENT_HPP
#include <string>
#include "IXWebSocket.h"

class WebsocketClient 
{
public:
  WebsocketClient(const ix::WebSocketHttpHeaders & header=ix::WebSocketHttpHeaders());
  ~WebsocketClient();
  void setHeartBeatPeriod(const int& HeartBeat);
  void start();
  void stop();
  
  ix::WebSocketSendInfo send(const std::string& data,bool binary = false,const ix::OnProgressCallback& onProgressCallback = nullptr);
  ix::WebSocketSendInfo sendBinary(const std::string& text,const ix::OnProgressCallback& onProgressCallback = nullptr);
  ix::WebSocketSendInfo sendText(const std::string& text,const ix::OnProgressCallback& onProgressCallback = nullptr);
  
private:
  static std::string m_Url;
  ix::WebSocket m_WebSocket;
  ix::WebSocketHttpHeaders m_Headers;
};
#endif
