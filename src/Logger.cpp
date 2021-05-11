#include "Logger.hpp"

#include "Exception.hpp"
#include "Message.hpp"
#include "StatusCode.hpp"
#include "spdlog/spdlog.h"

Logger::Logger(const std::string& name, const std::string& type) : Module(name,type,yaodaq::CLASS::Logger)
{
  skipConfigFile();
}

void Logger::OnMessage(const ix::WebSocketMessagePtr& msg)
{
  Message message;
  message.parse(msg->str);
  if(message.getType() == "Trace") { spdlog::trace("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo()); }
  else if(message.getType() == "Info")
  {
    spdlog::info("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo());
  }
  else if(message.getType() == "Debug")
  {
    spdlog::debug("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo());
  }
  else if(message.getType() == "Warning")
  {
    spdlog::warn("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo());
  }
  else if(message.getType() == "Critical")
  {
    spdlog::critical("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo());
  }
  else if(message.getType() == "Error")
  {
    spdlog::error("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo());
  }
  else if(message.getType() == "State")
  {
    spdlog::warn("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo());
  }
  else if(message.getType() == "Action")
  {
    spdlog::warn("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo());
  }
  else if(message.getType() == "Command")
  {
    spdlog::warn("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo());
  }
  else if(message.getType() == "Data")
  {
    spdlog::warn("Sent {0} Mo ; From : {1}; To : {2}", static_cast<float>(msg->wireSize / 1048576.0), message.getFrom(), message.getTo());
  }
  else
  {
    spdlog::info("Content : {0}; From : {1}; To : {2}", message.getContent(), message.getFrom(), message.getTo());
  }
}
