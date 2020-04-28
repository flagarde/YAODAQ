#include "SupressWarnings.hpp"
#include "spdlog.h"

#include <csignal>
#include <iostream>
#include <thread>

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
  void wait()
  {
    while(m_Continue) std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  virtual ~Interrupt() = default;

private:
  volatile static std::sig_atomic_t m_Continue;
  virtual void                      sigint(const int& signum)
  {
    std::cout << std::endl;
    spdlog::warn("Interrupt signal received.");
    m_Continue = 0;
  }
  virtual void sigterm(const int& signum)
  {
    std::cout << std::endl;
    spdlog::warn("Termination request, sent to the program ");
    m_Continue = 0;
  }
  virtual void sigsegv(const int& signum)
  {
    std::cout << std::endl;
    spdlog::critical("Invalid memory access (segmentation fault).");
    m_Continue = 0;
  }
  virtual void sigill(const int& signum)
  {
    std::cout << std::endl;
    spdlog::critical("Invalid program image.");
    m_Continue = 0;
  }
  virtual void sigabrt(const int& signum)
  {
    std::cout << std::endl;
    spdlog::error("Abnormal termination condition.");
    m_Continue = 0;
  }
  virtual void sigfpe(const int& signum)
  {
    std::cout << std::endl;
    spdlog::critical("Erroneous arithmetic operation.");
    m_Continue = 0;
  }
};
