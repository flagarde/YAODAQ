#include "Interrupt.hpp"

#include "spdlog.h"

#include <iostream>
#include <thread>

volatile std::sig_atomic_t Interrupt::m_Continue = 1;

int Interrupt::wait()
{
  while(m_Continue) std::this_thread::sleep_for(std::chrono::milliseconds(1));
  return m_Continue;
}

void Interrupt::sigint(const int& signum)
{
  std::cout << "\n";
  spdlog::warn("Interrupt signal received.");
  m_Continue = 0;
}

void Interrupt::sigterm(const int& signum)
{
  std::cout << "\n";
  spdlog::warn("Termination request, sent to the program ");
  m_Continue = 0;
}

void Interrupt::sigsegv(const int& signum)
{
  std::cout << "\n";
  spdlog::critical("Invalid memory access (segmentation fault).");
  m_Continue = 0;
}

void Interrupt::sigill(const int& signum)
{
  std::cout << "\n";
  spdlog::critical("Invalid program image.");
  m_Continue = 0;
}

void Interrupt::sigabrt(const int& signum)
{
  std::cout << "\n";
  spdlog::error("Abnormal termination condition.");
  m_Continue = 0;
}

void Interrupt::sigfpe(const int& signum)
{
  std::cout << "\n";
  spdlog::critical("Erroneous arithmetic operation.");
  m_Continue = 0;
}
