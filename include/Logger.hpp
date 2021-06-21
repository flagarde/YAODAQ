#pragma once

#include "Module.hpp"

namespace yaodaq
{

class Logger : public Module
{
public:
  Logger(const std::string& name = "", const std::string& type = "Default");
  virtual ~Logger() = default;
};

}
