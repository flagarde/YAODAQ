#include "Interrupt.hpp"
#include "spdlog.h"

#include <csignal>

volatile bool Interrupt::m_Continue = true;

Interrupt::Interrupt()
{
  std::signal(SIGTERM, [](int a) -> void {
    std::cout << "\n";
    spdlog::warn("Termination request, sent to the program ");
    m_Continue = false;
  });
  std::signal(SIGSEGV, [](int a) -> void {
    std::cout << "\n";
    spdlog::critical("Invalid memory access (segmentation fault).");
    m_Continue = false;
  });
  std::signal(SIGINT, [](int a) -> void {
    std::cout << "\n";
    spdlog::warn("Interrupt signal received.");
    m_Continue = false;
  });
  std::signal(SIGILL, [](int a) -> void {
    std::cout << "\n";
    spdlog::critical("Invalid program image.");
    m_Continue = false;
  });
  std::signal(SIGABRT, [](int a) -> void {
    std::cout << "\n";
    spdlog::error("Abnormal termination condition.");
    m_Continue = false;
  });
  std::signal(SIGFPE, [](int a) -> void {
    std::cout << "\n";
    spdlog::critical("Erroneous arithmetic operation.");
    m_Continue = false;
  });
}

int Interrupt::wait()
{
  while(m_Continue) std::this_thread::sleep_for(std::chrono::milliseconds(1));
  return m_Continue;
}
