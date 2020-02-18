#pragma once 

#include "Log.hpp"

#include "sinks/base_sink.h"
#include "WebsocketClient.hpp"
#include "details/null_mutex.h"

template<typename Mutex>
class WebSocketLoguer : public spdlog::sinks::base_sink <Mutex>
{
public:
    WebSocketLoguer(WebsocketClient& websocketclient,const std::string& from):m_WebSocketClient(websocketclient)
    {
      m_Log.setFrom(from);
    }
    void setWebSocketClient(const WebsocketClient& websocketclient)
    {
      m_WebSocketClient=websocketclient;
    }
    void setFrom(const std::string& from)
    {
      m_Log.setFrom(from);
    }
protected:
    void sink_it_(const spdlog::details::log_msg& msg) override
    {
      spdlog::memory_buf_t formatted;
      spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
      m_Log.setContent(fmt::to_string(formatted));
      m_WebSocketClient.sendText(m_Log.get());
    }
    void flush_() override 
    {
      
    }
private:
  WebsocketClient& m_WebSocketClient{nullptr};
  Log m_Log;
};


using WebSocketLoguer_mt = WebSocketLoguer<std::mutex>;
using WebSocketLoguer_st = WebSocketLoguer<spdlog::details::null_mutex>;
