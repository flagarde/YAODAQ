#include "Logger.hpp"

namespace yaodaq
{

Logger::Logger(const std::string& name, const std::string& type) : Module(name,type,yaodaq::CLASS::Logger)
{
  skipConfigFile();
}

}
