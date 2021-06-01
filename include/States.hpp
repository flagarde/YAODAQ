#pragma once

#include <cstdint>

enum class States : std::uint_least8_t
{
  UNINITIALIZED,
  INITIALIZED,
  CONNECTED,
  CONFIGURED,
  STARTED,
  PAUSED,
  STOPPED,
  CLEARED,
  DISCONNECTED,
  RELEASED,
  QUITED
};
