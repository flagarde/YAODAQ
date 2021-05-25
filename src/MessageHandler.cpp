#include "MessageHandler.hpp"

#include "Signals.hpp"
#include "magic_enum.hpp"

#include <thread>
#include <chrono>

namespace yaodaq
{

  MessageHandler::MessageHandler(const Identifier& identifier) : m_Identifier(identifier)
  {
    m_Interrupt.init();
  }

  void MessageHandler::onRaisingSignal()
  {
    static SIGNAL signal;
    do
    {
      signal=m_Interrupt.getSignal();
      std::this_thread::sleep_for(std::chrono::microseconds(5));
    }
    while(signal== yaodaq::SIGNAL::NO);
    int value = magic_enum::enum_integer(signal);
    if(value>=magic_enum::enum_integer(yaodaq::SEVERITY::Critical))
    {
      logger()->critical("Signal {} raised !",SignalName[signal]);
    }
    else if (value>=magic_enum::enum_integer(yaodaq::SEVERITY::Error))
    {
      logger()->error("Signal {} raised !",SignalName[signal]);
    }
    //Should be triggered by user so one character will appears -> Need to return line !
    else if (value>=magic_enum::enum_integer(yaodaq::SEVERITY::Warning))
    {
      fmt::print("\n");
      logger()->warn("Signal {} raised !",SignalName[signal]);
    }
    else if (value>=magic_enum::enum_integer(yaodaq::SEVERITY::Info))
    {
      fmt::print("\n");
      logger()->info("Signal {} raised !",SignalName[signal]);
    }
    else
    {
      fmt::print("\n");
      logger()->trace("Signal {} raised !",SignalName[signal]);
    }
    if(magic_enum::enum_integer(signal)>=magic_enum::enum_integer(SEVERITY::Critical)) std::exit(magic_enum::enum_integer(signal));
  }

};
