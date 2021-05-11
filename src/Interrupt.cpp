#include "Interrupt.hpp"

#include <csignal>

#include "Signals.hpp"

namespace yaodaq
{

  volatile SIGNAL Interrupt::m_Signal = SIGNAL::NO;

  void Interrupt::init()
  {
    std::signal(SIGTERM, [](int a) -> void { m_Signal = SIGNAL::TERM ; });
    std::signal(SIGSEGV, [](int a) -> void { m_Signal = SIGNAL::SEGV ; });
    std::signal(SIGINT, [](int a) -> void { m_Signal = SIGNAL::INT ; });
    std::signal(SIGILL, [](int a) -> void { m_Signal = SIGNAL::ILL ; });
    std::signal(SIGABRT, [](int a) -> void { m_Signal = SIGNAL::ABRT ; });
    std::signal(SIGFPE, [](int a) -> void { m_Signal = SIGNAL::FPE ; });
  }

  SIGNAL Interrupt::getSignal()
  {
    return m_Signal;
  }

};
