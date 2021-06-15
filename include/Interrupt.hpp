#pragma once

#include <atomic>

namespace yaodaq
{

  enum class SIGNAL;

  class Interrupt
  {
  public:
    void init() const;
    SIGNAL getSignal() const;
    ~Interrupt();
  private:
    volatile static std::atomic<SIGNAL> m_Signal;
  };

};
