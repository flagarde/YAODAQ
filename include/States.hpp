#pragma once

#include <cstdint>

enum class STATE : std::uint_least8_t
{
  Uninitialized,
  Initialized,
  Connected,
  Configured,
  Started,
  Paused,
  Stopped,
  Cleared,
  Disconnected,
  Released,
  Quited
};
