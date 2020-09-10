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
    [[nodiscard]] std::uint_least32_t line() const { return 0; }
    [[nodiscard]] std::uint_least32_t column() const { return 0; }
    [[nodiscard]] const char*    file_name() const { return "Use a compiler with source_location support to obtain these informations !"; }
    [[nodiscard]] const char*    function_name() const { return "Use a compiler with source_location support to obtain these informations !"; }
  };
#endif

class SourceLocation
{
public:
#if __has_include(<experimental/source_location>)
  explicit SourceLocation(const source_location& location = source_location::current());
#elif __has_include(<source_location>)
  SourceLocation(const source_location& location = current());
#else
  SourceLocation()=default;
#endif
  [[nodiscard]] std::uint_least32_t getLine() const;
  [[nodiscard]] std::uint_least32_t getColumn() const;
  [[nodiscard]] const char*               getFileName() const;
  [[nodiscard]] const char*               getFunctionName() const;

private:
  source_location m_Location;
};
