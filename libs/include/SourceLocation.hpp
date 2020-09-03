#pragma once

#include "Internal.hpp"

#if __has_include(<source_location>)
  #include <source_location>
  using std::source_location;
  #elif __has_include(<experimental/source_location>)
#include <experimental/source_location>
  using std::experimental::source_location;
#else
  class source_location
  {
  public:
    std::uint_least32_t line() const { return 0; }
    std::uint_least32_t column() const { return 0; }
    const char*    file_name() const { return "Use a compiler with source_location support to obtain these informations !"; }
    const char*    function_name() const { return "Use a compiler with source_location support to obtain these informations !"; }
  };
#endif

class SourceLocation
{
public:
  #if __has_include(<experimental/source_location>)
    SourceLocation(const source_location& location = source_location::current());
  #elif __has_include(<source_location>)
    SourceLocation(const source_location& location = current());
  #else
    SourceLocation()=default;
  #endif
  const std::uint_least32_t getLine() const;
  const std::uint_least32_t getColumn() const;
  const char*               getFileName() const;
  const char*               getFunctionName() const;

private:
  source_location m_Location;
};
