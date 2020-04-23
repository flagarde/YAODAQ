#pragma once

#include "SupressWarnings.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>

using std::size_t;

using std::int16_t;
using std::int32_t;
using std::int64_t;
using std::int8_t;

using std::uint16_t;
using std::uint32_t;
using std::uint64_t;
using std::uint8_t;

#define PLATFORM_LINUX  1
#define PLATFORM_MACOSX 2
#define PLATFORM_CYGWIN 3
#define PLATFORM_WIN32  4

#define PLATFORM_IS(P) (PLATFORM == PLATFORM_##P)
