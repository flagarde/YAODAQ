#include "Exception.hpp"

#if experimental_have_source_location == 1
Exception::Exception(const int& code, const std::string& message,
                     std::experimental::source_location location)
    : m_Code(code), m_Message(message), m_Location(location)
{
  constructMessage();
}
#elif have_source_location == 1
Exception::Exception(const int& code, std::source_location location)
    : m_Code(code), m_Location(location)
{
  constructMessage();
}
#else
Exception::Exception(const int& code): m_Code(code)
{
  constructMessage();
}
#endif
const char* Exception::what() const noexcept
{
  return m_Message.c_str();
}

const char* Exception::errorStrings(const std::int_least32_t& code)
{
  return "";
}

void Exception::constructMessage()
{
  m_Message = "Error " + std::to_string(m_Code) + "\n\t-> " + m_Message;
#if have_source_location == 1
  m_Message +=
      (" in function \"" + std::string(functionName()) + "\" in file \"" +
       std::string(fileName()) + "\" line " + std::to_string(line()))
          .c_str();
#endif
}

Exception::~Exception(){};

const std::uint_least32_t Exception::line() const
{
  return m_Location.line();
}

const std::uint_least32_t Exception::column() const
{
  return m_Location.column();
}

const char* Exception::fileName() const
{
  return m_Location.file_name();
}

const char* Exception::functionName() const
{
  return m_Location.function_name();
}

const std::int_least32_t Exception::code() const
{
  return m_Code;
}
