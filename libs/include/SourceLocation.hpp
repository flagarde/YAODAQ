#pragma once
#include "Internal.hpp"

#if __has_include(<source_location>)
  #define have_source_location 1
  #include <source_location>
using std::source_location;
#elif __has_include(<experimental/source_location>)
  #define have_experimental_source_location 1
  #define have_source_location              1
  #include <experimental/source_location>
using std::experimental::source_location;
#else
class source_location
{
public:
  uint_least32_t line() const { return 0; }
  uint_least32_t column() const { return 0; }
  const char*    file_name() const { return "Use a compiler with source_location support to obtain these informations !"; }
  const char*    function_name() const { return "Use a compiler with source_location support to obtain these informations !"; }
};
#endif

class SourceLocation
{
public:
#if have_experimental_source_location == 1
  SourceLocation(const source_location& location = source_location::current()): m_Location(location) {}
#elif have_source_location == 1
  SourceLocation(const source_location& location = current()): m_Location(location) {}
#else
  SourceLocation() {}
#endif
  const uint_least32_t getLine() const;
  const uint_least32_t getColumn() const;
  const char*          getFileName() const;
  const char*          getFunctionName() const;

private:
  source_location m_Location;
};

#undef have_source_location
#undef have_experimental_source_location
