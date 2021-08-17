#pragma once
// clang-format off
#if defined(_MSC_VER)
    #define DISABLE_WARNING_PUSH           __pragma(warning( push ))
    #define DISABLE_WARNING_POP            __pragma(warning( pop ))
    #define DISABLE_WARNING(warningNumber) __pragma(warning( disable :  warningNumber))
#elif defined(__clang__) || defined(__llvm__)
    #define DO_PRAGMA(X) _Pragma(#X)
    #define DISABLE_WARNING_PUSH           DO_PRAGMA(clang diagnostic push)
    #define DISABLE_WARNING_POP            DO_PRAGMA(clang diagnostic pop)
    #define DISABLE_WARNING(warningName)   DO_PRAGMA(clang diagnostic ignored #warningName)
#elif defined(__GNUC__)
    #define DO_PRAGMA(X) _Pragma(#X)
    #define DISABLE_WARNING_PUSH           DO_PRAGMA(GCC diagnostic push)
    #define DISABLE_WARNING_POP            DO_PRAGMA(GCC diagnostic pop)
    #define DISABLE_WARNING(warningName)   DO_PRAGMA(GCC diagnostic ignored #warningName)
#else
    #define DISABLE_WARNING_PUSH
    #define DISABLE_WARNING_POP
#endif
#include "Warnings.hpp"
// clang-format on
