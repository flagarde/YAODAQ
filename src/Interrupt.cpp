#include "Interrupt.hpp"

#include "spdlog/spdlog.h"

#include <csignal>
#include <thread>

volatile bool Interrupt::m_Continue = true;

Interrupt::Interrupt()
{
  std::signal(SIGTERM, [](int a) -> void {
    fmt::print("\n");
    spdlog::warn("Termination request, sent to the program ");
    m_Continue = false;
  });
  std::signal(SIGSEGV, [](int a) -> void {
    fmt::print("\n");
    spdlog::critical("Invalid memory access (segmentation fault).");
    m_Continue = false;
  });
  std::signal(SIGINT, [](int a) -> void {
    fmt::print("\n");
    spdlog::warn("Interrupt signal received.");
    m_Continue = false;
  });
  std::signal(SIGILL, [](int a) -> void {
    fmt::print("\n");
    spdlog::critical("Invalid program image.");
    m_Continue = false;
  });
  std::signal(SIGABRT, [](int a) -> void {
    fmt::print("\n");
    spdlog::error("Abnormal termination condition.");
    m_Continue = false;
  });
  std::signal(SIGFPE, [](int a) -> void {
    fmt::print("\n");
    spdlog::critical("Erroneous arithmetic operation.");
    m_Continue = false;
  });
}

int Interrupt::wait()
{
  while(m_Continue) std::this_thread::sleep_for(std::chrono::milliseconds(1));
  return m_Continue;
}
