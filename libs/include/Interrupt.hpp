#pragma once

class Interrupt
{
public:
  Interrupt();
  int wait();
  virtual ~Interrupt() = default;

private:
  volatile static bool m_Continue;
};
