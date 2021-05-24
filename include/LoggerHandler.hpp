#pragma once

#include <memory>
#include <vector>
#include <string>

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
    LoggerHandler();

    std::shared_ptr<spdlog::logger> logger();

    void setName(const std::string&);
    void addSink(const spdlog::sink_ptr&);
    void clearSinks();
    void setVerbosity(const std::string&);
  private:
    std::shared_ptr<spdlog::logger> m_Logger{nullptr};
    void init();
    std::vector<spdlog::sink_ptr> m_Sinks;
    std::string m_Name;
    spdlog::level::level_enum m_Verbosity{spdlog::level::info};
  };
};
