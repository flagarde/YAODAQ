#include "LoggerHandler.hpp"

#include "spdlog/spdlog.h"

#include <iostream>

namespace yaodaq
{
  LoggerHandler::LoggerHandler()
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
    if(verbosity == "off") m_Verbosity = spdlog::level::off;
    else if(verbosity == "trace") m_Verbosity = spdlog::level::trace;
    else if(verbosity == "info") m_Verbosity = spdlog::level::info;
    else if(verbosity == "debug") m_Verbosity = spdlog::level::debug;
    else if(verbosity == "warning") m_Verbosity = spdlog::level::warn;
    else if(verbosity == "critical") m_Verbosity = spdlog::level::critical;
  }

  void LoggerHandler::setName(const std::string& name)
  {
    m_Name=name;
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
    m_Logger=std::make_shared<spdlog::logger>(m_Name, std::begin(m_Sinks), std::end(m_Sinks));
    m_Logger->set_level(m_Verbosity);
  }

};
