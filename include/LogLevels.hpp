#pragma once

#include<cstdint>

namespace yaodaq
{

  enum class LEVEL : std::uint_least8_t
  {
    Trace,
    Debug,
    Info,
    Warn,
    Error,
    Critical
  };

}
