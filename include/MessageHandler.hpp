#pragma once

#include "LoggerHandler.hpp"
#include "Interrupt.hpp"

namespace yaodaq
{

  class MessageHandler
  {
  public:
    MessageHandler(const Identifier&);
    void setVerbosity(const std::string& verbosity)
    {
      m_LoggerHandler.setVerbosity(verbosity);
    }
    std::shared_ptr<spdlog::logger> logger()
    {
      return m_LoggerHandler.logger();
    }


    void clearSinks()
    {
      m_LoggerHandler.clearSinks();
    }

    void addSink(const spdlog::sink_ptr& sink)
    {
      m_LoggerHandler.addSink(sink);
    }


    Identifier getIdentifier()
    {
      return m_LoggerHandler.getIdentifier();
    }


    //Action to do when a signal as been raised on the client/server
    void onRaisingSignal();


  protected:
    LoggerHandler m_LoggerHandler;
    Interrupt  m_Interrupt;
  };
};
