#pragma once

namespace yaodaq
{

  enum class ACTION : std::uint_least8_t
  {
    Initialize,
    Connect,
    Configure,
    Start,
    Pause,
    Stop,
    Clear,
    Disconnect,
    Release,
    Quit
  };

}
