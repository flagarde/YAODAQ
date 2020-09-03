#include "SourceLocation.hpp"

const std::uint_least32_t SourceLocation::getLine() const
{
  return m_Location.line();
}

const std::uint_least32_t SourceLocation::getColumn() const
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
