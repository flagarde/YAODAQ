#pragma once

#include "LoggerHandler.hpp"
#include "Interrupt.hpp"

namespace yaodaq
{

  class MessageHandler
  {
  public:
    MessageHandler();
    void setVerbosity(const std::string& verbosity)
    {
      m_LoggerHandler.setVerbosity(verbosity);
    }
    std::shared_ptr<spdlog::logger> logger()
    {
      return m_LoggerHandler.logger();
    }

    //Action to do when a signal as been raised on the client/server
    void onRaisingSignal();


  protected:
    LoggerHandler m_LoggerHandler;
    std::string m_Name();
    Interrupt  m_Interrupt;
  };
};
