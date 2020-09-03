#pragma once

#include <atomic>

class Interrupt
{
public:
  Interrupt();
  int wait() noexcept;
  virtual ~Interrupt() noexcept = default;

private:
  volatile static std::atomic<bool> m_Continue;
};
