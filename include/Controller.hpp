#pragma once

#include <string>

#include "Module.hpp"
#include "Exception.hpp"
#include "Message.hpp"
#include "StatusCode.hpp"

namespace yaodaq
{

class Controller : public Module
{
public:
  Controller(const std::string& name = "", const std::string& type = "Controller");
  virtual ~Controller() = default;
  void                  sendAction(const std::string& action);
  void                  sendCommand(const std::string& command);
};

};
