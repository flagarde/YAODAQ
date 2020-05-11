#include "SupressWarnings.hpp"

#include <csignal>

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
  int wait();
  virtual ~Interrupt() = default;

protected:
  volatile static std::sig_atomic_t m_Continue;
  virtual void                      sigint(const int& signum);
  virtual void                      sigterm(const int& signum);
  virtual void                      sigsegv(const int& signum);
  virtual void                      sigill(const int& signum);
  virtual void                      sigabrt(const int& signum);
  virtual void                      sigfpe(const int& signum);
};
