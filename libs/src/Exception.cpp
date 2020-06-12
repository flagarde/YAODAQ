#include "Exception.hpp"

Exception::Exception(const StatusCode& statusCode, const std::string& description, const SourceLocation& location)
    : m_Code(static_cast<std::int_least32_t>(statusCode)), m_Description(description), m_SourceLocation(location)
{
  constructMessage();
}

Exception::~Exception(){};

const char* Exception::what() const noexcept
{
  return m_Message.c_str();
}

const uint_least32_t Exception::getLine() const
{
  return m_SourceLocation.getLine();
}

const uint_least32_t Exception::getColumn() const
{
  return m_SourceLocation.getColumn();
}

const char* Exception::getFileName() const
{
  return m_SourceLocation.getFileName();
}

const char* Exception::getFunctionName() const
{
  return m_SourceLocation.getFunctionName();
}

const char* Exception::getDescription() const
{
  return m_Description.c_str();
}

const int_least32_t Exception::getCode() const
{
  return m_Code;
}

Exception::Exception(const int_least32_t& code, const std::string& description, const SourceLocation& location): m_Code(static_cast<std::int_least32_t>(code)), m_Description(description), m_SourceLocation(location)
{
  constructMessage();
}

void Exception::constructMessage() noexcept
{
  m_Message = "Error ! :";
  m_Message += "\n\t[Code] : " + std::to_string(m_Code);
  m_Message += "\n\t[Description] : " + m_Description;
  m_Message += "\n\t[File] : " + std::string(getFileName());
  m_Message += "\n\t[Function] : " + std::string(getFunctionName());
  m_Message += "\n\t[Line] : " + std::to_string(getLine());
  m_Message += "\n\t[Column] : " + std::to_string(getColumn());
  m_Message += "\n";
}
