#pragma once

#include <memory>
#include <vector>
#include <string>

#include "Identifier.hpp"

#include "spdlog/spdlog.h"

/*namespace spdlog
{
 class logger;
 using sink_ptr = std::shared_ptr<sinks::sink>;
}*/

namespace yaodaq
{
  class LoggerHandler
  {
  public:
    LoggerHandler(const Identifier&);

    std::shared_ptr<spdlog::logger> logger();

    void addSink(const spdlog::sink_ptr&);
    void clearSinks();
    void setVerbosity(const std::string&);

    Identifier getIdentifier()
    {
      return m_Identifier;
    }
  private:
    Identifier m_Identifier;
    std::shared_ptr<spdlog::logger> m_Logger{nullptr};
    void init();
    std::vector<spdlog::sink_ptr> m_Sinks;
    spdlog::level::level_enum m_Verbosity{spdlog::level::trace};


  };
};
