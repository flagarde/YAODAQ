#include "Exception.hpp"

std::string  Exception::m_Format{"\n\t[Code] : {Code}\n\t[Description] : {Description}\n\t[File] : {File}\n\t[Function] : {Function}\n\t[Line] : {Line}\n\t[Column] : {Column}\n"};

fmt::text_style Exception::m_Style={fg(fmt::color::crimson) | fmt::emphasis::bold};

Exception::Exception(const StatusCode& statusCode, std::string description,const SourceLocation& location)
: SourceLocation(location), m_Code(static_cast<std::int_least32_t>(statusCode)), m_Description(std::move(description))
{
  constructMessage();
}

const char* Exception::what() const noexcept
{
  return m_Message.c_str();
}

const char* Exception::description() const noexcept
{
  return m_Description.c_str();
}

int_least32_t Exception::code() const noexcept
{
  return m_Code;
}

Exception::Exception(const int_least32_t& code, std::string description,const SourceLocation& location):SourceLocation(location),  m_Code(static_cast<int_least32_t>(code)), m_Description(std::move(description))
{
  constructMessage();
}

void Exception::constructMessage()
{
  m_Message = fmt::format(m_Style, m_Format, fmt::arg("Code", m_Code), fmt::arg("Description", m_Description), fmt::arg("File", file_name()), fmt::arg("Function", function_name()), fmt::arg("Column", column()), fmt::arg("Line", line()));
}
