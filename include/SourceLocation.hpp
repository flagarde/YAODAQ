#pragma once

#include <cstdint>

#include "Internal.hpp"

#if __has_include(<experimental/source_location>)
  #include <experimental/source_location>
  using source_location = std::experimental::source_location;
#elif __has_include(<source_location>)
  #include <source_location>
  using source_location = std::source_location;
#else
  class source_location
  {
  public:
    source_location()=default;
    source_location(const std::uint_least32_t& line,const std::uint_least32_t& column,const char* fileName,const char* functionName):m_Line(line),m_Column(column),m_FileName(fileName),m_FunctionName(functionName)
    {
    }
    [[nodiscard]] std::uint_least32_t line() const { return m_Line; }
    [[nodiscard]] std::uint_least32_t column() const { return m_Column; }
    [[nodiscard]] const char*    file_name() const { return m_FileName; }
    [[nodiscard]] const char*    function_name() const { return m_FunctionName; }
  private:
    std::uint_least32_t m_Line{0};
    std::uint_least32_t m_Column{0};
    const char* m_FileName{"\0"};
    const char* m_FunctionName{"\0"};
  };
#endif

class SourceLocation : public source_location
{
public:
#if __has_include(<experimental/source_location>) or __has_include(<source_location>)
  SourceLocation(const source_location sourceLocation=source_location::current()) : source_location(sourceLocation){}
#else
  SourceLocation(const std::uint_least32_t& line,const std::uint_least32_t& column,const char* fileName,const char* functionName) : source_location(line,column,fileName,functionName){}
  #if defined (__GNUC__)
  #  define __FUNCTION__     ((const char*) (__PRETTY_FUNCTION__))
  #elif defined (__STDC_VERSION__) && __STDC_VERSION__ >= 19901L
  #  define __FUNCTION__     ((const char*) (__func__))
  #endif
  #define SourceLocation(args...) SourceLocation( static_cast<const std::uint_least32_t>(__LINE__) , static_cast<const std::uint_least32_t>(0) , static_cast<const char*>(__FILE__) , static_cast<const char*>(__FUNCTION__) )
#endif
};
