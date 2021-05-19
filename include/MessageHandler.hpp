#pragma once

#include "LoggerHandler.hpp"

namespace ix
{
  class ConnectionState;
  class WebSocket;
  class WebSocketMessage;
  using WebSocketMessagePtr = std::unique_ptr<WebSocketMessage>;
};

namespace yaodaq
{
  class MessageHandler
  {
  public:
    void setVerbosity(const std::string& verbosity)
    {
      m_LoggerHandler.setVerbosity(verbosity);
    }
    std::shared_ptr<spdlog::logger> logger()
    {
      return m_LoggerHandler.logger();
    }
  protected:
    LoggerHandler m_LoggerHandler;
    std::string m_Name();
  };
};
