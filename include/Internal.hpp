#pragma once

#define PLATFORM_LINUX  1
#define PLATFORM_MACOSX 2
#define PLATFORM_CYGWIN 3
#define PLATFORM_WIN32  4

#define PLATFORM_IS(P) (PLATFORM == PLATFORM_##P)

#include "SupressWarnings.hpp"
