#include "SupressWarnings.hpp"
#include "spdlog.h"

#include <csignal>
#include <functional>

class Interrupt
{
public:
  Interrupt()
  {
    DISABLE_WARNING_PUSH
    DISABLE_WARNING_PMFCONVERSIONS
    std::signal(SIGTERM, (void (*)(int))(&Interrupt::sigterm));
    std::signal(SIGSEGV, (void (*)(int))(&Interrupt::sigsegv));
    std::signal(SIGINT, (void (*)(int))(&Interrupt::sigint));
    std::signal(SIGILL, (void (*)(int))(&Interrupt::sigill));
    std::signal(SIGABRT, (void (*)(int))(&Interrupt::sigabrt));
    std::signal(SIGFPE, (void (*)(int))(&Interrupt::sigfpe));
    DISABLE_WARNING_POP
  }
  virtual ~Interrupt() {}

private:
  virtual void sigint(const int& signum)
  {
    spdlog::warn("Interrupt signal received.\n");
    std::exit(signum);
  }
  virtual void sigterm(const int& signum)
  {
    spdlog::warn("Termination request, sent to the program \n");
    std::exit(signum);
  }
  virtual void sigsegv(const int& signum)
  {
    spdlog::critical("Invalid memory access (segmentation fault).\n");
    std::exit(signum);
  }
  virtual void sigill(const int& signum)
  {
    spdlog::critical("Invalid program image.\n");
    std::exit(signum);
  }
  virtual void sigabrt(const int& signum)
  {
    spdlog::error("Abnormal termination condition.\n");
    std::exit(signum);
  }
  virtual void sigfpe(const int& signum)
  {
    spdlog::critical("Erroneous arithmetic operation.\n");
    std::exit(signum);
  }
};
