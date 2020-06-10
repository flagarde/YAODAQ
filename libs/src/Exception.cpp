#include "Exception.hpp"

#include "StatusCode.hpp"

#if experimental_have_source_location == 1
Exception::Exception(const StatusCode& statusCode, const std::string& description, const std::experimental::source_location& location)
    : m_Code(static_cast<std::int_least32_t>(statusCode)), m_Description(description), m_Location(location)
{
  constructMessage();
}
#elif have_source_location == 1
Exception::Exception(const StatusCode& statusCode, const std::string& description, const std::source_location& location): m_Code(static_cast<std::int32_t>(statusCode)), m_Description(description), m_Location(location)
{
  constructMessage();
}
#else
Exception::Exception(const StatusCode& statusCode, const std::string& description): m_Code(static_cast<std::int32_t>(statusCode)), m_Description(description)
{
  constructMessage();
}
#endif

Exception::~Exception(){};

const char* Exception::what() const noexcept
{
  return m_Message.c_str();
}

#if have_source_location == 1 || experimental_have_source_location == 1
const uint_least32_t Exception::getLine() const
{
  return m_Location.line();
}

const uint_least32_t Exception::getColumn() const
{
  return m_Location.column();
}

const char* Exception::getFileName() const
{
  return m_Location.file_name();
}

const char* Exception::getFunctionName() const
{
  return m_Location.function_name();
}
#endif

const char* Exception::getDescription() const
{
  return m_Description.c_str();
}

const int_least32_t Exception::getCode() const
{
  return m_Code;
}

#if experimental_have_source_location == 1
Exception::Exception(const int_least32_t& code, const std::string& description, const std::experimental::source_location& location)
    : m_Code(static_cast<std::int_least32_t>(code)), m_Description(description), m_Location(location)
{
  constructMessage();
}
#elif have_source_location == 1
Exception::Exception(const int_least32_t& code, const std::string& description, const std::source_location& location): m_Code(static_cast<std::int_least32_t>(code)), m_Description(description), m_Location(location)
{
  constructMessage();
}
#else
Exception::Exception(const int_least32_t& code, const std::string& description): m_Code(static_cast<std::int_least32_t>(code)), m_Description(description)
{
  constructMessage();
}
#endif

void Exception::constructMessage() noexcept
{
  m_Message = "Error ! :";
  m_Message += "\n\t[Code] : " + std::to_string(m_Code);
  m_Message += "\n\t[Description] : " + m_Description;
#if have_source_location == 1 || experimental_have_source_location == 1
  m_Message += "\n\t[File] : " + std::string(getFileName());
  m_Message += "\n\t[Function] : " + std::string(getFunctionName());
  m_Message += "\n\t[Line] : " + std::to_string(getLine());
  m_Message += "\n\t[Column] : " + std::to_string(getColumn());
#endif
  m_Message += "\n";
}
