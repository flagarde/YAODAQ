#pragma once

#include "LoggerHandler.hpp"
#include "Interrupt.hpp"
#include "Message.hpp"

namespace yaodaq
{

  class MessageHandler
  {
  public:
    MessageHandler(const Identifier&);

    //******************* LOGS ******************************//
    // Log messages
    template<typename... Args> inline void log(const LEVEL& level, fmt::string_view str, const Args&... args)
    {
      Log log(level,fmt::format(str,args...));
      log.setFrom(getIdentifier().get());
      sendLog(log);
    }
    template<typename... Args> inline void log(const std::string& level, fmt::string_view str, const Args&... args)
    {
      Log log(level,fmt::format(str,args...));
      log.setFrom(getIdentifier().get());
      sendLog(log);
    }
    template<typename... Args> inline void log(fmt::string_view fmt, const Args&... args)
    {
      log(LEVEL::Info,fmt,args...);
    }

    // LEVELS
    template<typename... Args> inline void trace(fmt::string_view fmt, const Args&... args)
    {
      log(LEVEL::Trace,fmt,args...);
    }
    template<typename... Args> inline void debug(fmt::string_view fmt, const Args&... args)
    {
      log(LEVEL::Debug,fmt,args...);
    }
    template<typename... Args> inline void info(fmt::string_view fmt, const Args&... args)
    {
      log(LEVEL::Info,fmt,args...);
    }
    template<typename... Args> inline void warn(fmt::string_view fmt, const Args&... args)
    {
      log(LEVEL::Warn,fmt,args...);
    }
    template<typename... Args> inline void error(fmt::string_view fmt, const Args&... args)
    {
      log(LEVEL::Error,fmt,args...);
    }
    template<typename... Args> inline void critical(fmt::string_view fmt, const Args&... args)
    {
      log(LEVEL::Critical,fmt,args...);
    }

    //******************************************************//

    //*********************** Except ***********************//
    virtual void printException(const Exception& exception)
    {
      logger()->error(fmt::format(fg(fmt::color::red) | fmt::emphasis::bold,"Exception :\n{}",exception.what()));
    }
    virtual void printException(const Except&);
    //******************************************************//


    virtual void printConnectionError(const ConnectionError&);

    virtual void printPing(const Ping&);
    virtual void printPong(const Pong&);


    virtual void printState(const State&);
    virtual void onState(const State&);

    virtual void printAction(const Action&);
    virtual void onAction(const Action&);

    virtual void printData(const Data&);
    virtual void onData(const Data&);


    virtual void onPing(const Ping&);
    virtual void onPong(const Pong&);


    virtual void onClose(const Close&);


    //Dumb sender (Only print the log)
    virtual void sendLog(Log& log)
    {
      printLog(log);
    }

    virtual void sendException(Except&);

    virtual void onException(const Except&);

    virtual void printOpen(const Open&);
    virtual void printClose(const Close&);



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
    void printLog(const Log& log);

    //When it receive a Log
    virtual void onLog(const Log& log);

    LoggerHandler m_LoggerHandler;
    Interrupt  m_Interrupt;
  };
};
