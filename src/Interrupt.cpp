#include "Interrupt.hpp"

#include <csignal>

#include "Signals.hpp"

namespace yaodaq
{

  volatile std::atomic<SIGNAL> Interrupt::m_Signal = SIGNAL::NO;

  void Interrupt::init() const
  {
    std::signal(SIGTERM, [](int a) -> void { m_Signal.store(SIGNAL::TERM) ; });
    std::signal(SIGSEGV, [](int a) -> void { m_Signal.store(SIGNAL::SEGV) ; });
    std::signal(SIGINT, [](int a) -> void { m_Signal.store(SIGNAL::INT) ; });
    std::signal(SIGILL, [](int a) -> void { m_Signal.store(SIGNAL::ILL) ; });
    std::signal(SIGABRT, [](int a) -> void { m_Signal.store(SIGNAL::ABRT) ; });
    std::signal(SIGFPE, [](int a) -> void { m_Signal.store(SIGNAL::FPE) ; });
  }

  SIGNAL Interrupt::getSignal() const
  {
    return m_Signal.load();
  }

};
