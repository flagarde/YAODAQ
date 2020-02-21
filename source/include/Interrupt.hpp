#include "spdlog.h"
#include <csignal>
#include <functional>
#include "SupressWarnings.hpp"

class Interrupt
{
public:
  Interrupt()
  {
    DISABLE_WARNING_PUSH
    DISABLE_WARNING(-Wpmf-conversions)
    std::signal(SIGINT,(void(*)(int))(&Interrupt::signalHandler));  
    DISABLE_WARNING_POP
  }
private:
  void signalHandler(const int& signum ) 
  {
   spdlog::warn("Interrupt signal ({}) received.\n",signum);
   std::exit(signum);  
  }
};
