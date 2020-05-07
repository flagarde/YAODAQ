#include "Exception.hpp"

#if(PLATFORM_IS(LINUX) || PLATFORM_IS(MACOSX)) && defined(BACKTRACE)
  #include "execinfo.h"
#endif

#if experimental_have_source_location == 1
Exception::Exception(const StatusCode& statusCode, const std::string& message, std::experimental::source_location location): m_Code(static_cast<std::int32_t>(statusCode)), m_Message(message), m_Location(location)
{
  constructMessage();
  createBackTrace();
}
#elif have_source_location == 1
Exception::Exception(const StatusCode& statusCode, std::source_location location): m_Code(static_cast<std::int32_t>(statusCode)), m_Location(location)
{
  constructMessage();
  createBackTrace();
}
#else
Exception::Exception(const StatusCode& statusCode): m_Code(static_cast<std::int32_t>(statusCode))
{
  constructMessage();
  createBackTrace();
}
#endif

void Exception::createBackTrace()
{
#if(PLATFORM_IS(LINUX) || PLATFORM_IS(MACOSX)) && defined(BACKTRACE)
  const size_t maxDepth = 100;
  void*        stackAddresses[maxDepth];

  size_t stackDepth   = backtrace(stackAddresses, maxDepth);
  char** stackStrings = backtrace_symbols(stackAddresses, stackDepth);

  for(size_t i = 1; i < stackDepth; ++i)
  {
    m_BackTrace += "\t-> " + stackStrings[i];
    m_BackTrace += "\n";
  }

  free(stackStrings);  // malloc()ed by backtrace_symbols
#endif
}

#if experimental_have_source_location == 1
Exception::Exception(const int& code, const std::string& message, std::experimental::source_location location): m_Code(code), m_Message(message), m_Location(location)
{
  constructMessage();
  createBackTrace();
}
#elif have_source_location == 1
Exception::Exception(const int& code, std::source_location location): m_Code(code), m_Location(location)
{
  constructMessage();
  createBackTrace();
}
#else
Exception::Exception(const int& code): m_Code(code)
{
  constructMessage();
  createBackTrace();
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
  m_Message = "Error " + std::to_string(m_Code) + "\n\t[Error] : " + m_Message;
#if have_source_location == 1
  m_Message += "\n\t[File] : " + std::string(getFileName());
  m_Message += "\n\t[Function] : " + std::string(getFunctionName());
  m_Message += "\n\t[Line] : " + std::to_string(getLine());
  m_Message += "\n\t[Column] : " + std::to_string(getColumn());
#endif
  if(m_BackTrace != "")
  {
    m_Message += "\n  \t [Backtrace] :";
    m_Message += m_BackTrace;
  }
}

const std::string& Exception::getBackTrace() const
{
  return m_BackTrace;
}

Exception::~Exception(){};

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

const int Exception::getCode() const
{
  return m_Code;
}
