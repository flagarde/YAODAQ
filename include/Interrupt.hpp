#pragma once

namespace yaodaq
{

  enum class SIGNAL;

  class Interrupt
  {
  public:
    void init();
    SIGNAL getSignal();

  private:
    volatile static SIGNAL m_Signal;
  };

};
