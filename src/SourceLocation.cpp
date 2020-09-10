#include "SourceLocation.hpp"

#if __has_include(<experimental/source_location>)
  SourceLocation::SourceLocation(const source_location& location): m_Location(location) {}
#elif __has_include(<source_location>)
  SourceLocation::SourceLocation(const source_location& location): m_Location(location) {}
#endif

std::uint_least32_t SourceLocation::getLine() const
{
  return m_Location.line();
}

std::uint_least32_t SourceLocation::getColumn() const
{
  return m_Location.column();
}

const char* SourceLocation::getFileName() const
{
  return m_Location.file_name();
}

const char* SourceLocation::getFunctionName() const
{
  return m_Location.function_name();
}
