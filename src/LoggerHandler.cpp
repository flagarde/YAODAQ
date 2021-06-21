#include "LoggerHandler.hpp"

#include "spdlog/spdlog.h"

#include <iostream>

namespace yaodaq
{
  LoggerHandler::LoggerHandler(const Identifier& identifier) : m_Identifier(identifier)
  {
    init();
  }

  void LoggerHandler::clearSinks()
  {
    m_Sinks.clear();
    init();
  }

  void LoggerHandler::setVerbosity(const std::string& verbosity)
  {
    if(verbosity == "Off") m_Verbosity = spdlog::level::off;
    else if(verbosity == "Trace") m_Verbosity = spdlog::level::trace;
    else if(verbosity == "Debug") m_Verbosity = spdlog::level::debug;
    else if(verbosity == "Info") m_Verbosity = spdlog::level::info;
    else if(verbosity == "Warn") m_Verbosity = spdlog::level::warn;
    else if(verbosity == "Error") m_Verbosity = spdlog::level::err;
    else if(verbosity == "Critical") m_Verbosity = spdlog::level::critical;
    init();
  }

  std::shared_ptr<spdlog::logger> LoggerHandler::logger()
  {
    return std::shared_ptr<spdlog::logger>(m_Logger);
  }

  void LoggerHandler::addSink(const spdlog::sink_ptr& sink)
  {
    m_Sinks.push_back(sink);
    init();
  }

  void LoggerHandler::init()
  {
    m_Logger=std::make_shared<spdlog::logger>(m_Identifier.get(), std::begin(m_Sinks), std::end(m_Sinks));
    m_Logger->set_level(m_Verbosity);
  }

}
