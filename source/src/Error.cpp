#include "Error.hpp"

#if experimental_have_source_location == 1
Error::Error(const int &code, const std::string &message,
             std::experimental::source_location loc)
    : m_code(code), m_message(message), location(loc) {
  construct_message();
}
#elif have_source_location == 1
Error::Error(const int &code, std::source_location loc)
    : m_code(code), location(loc) {
  construct_message();
}
#else
Error::Error(const int &code) : m_code(code) { construct_message(); }
#endif
const char *Error::what() const noexcept { return m_message.c_str(); }

const char *Error::ErrorStrings(const std::int_least32_t &code) { return ""; }

void Error::construct_message() {
  m_message = "Error " + std::to_string(m_code) + "\n\t-> " + m_message;
#if have_source_location == 1
  m_message +=
      (" in function \"" + std::string(function_name()) + "\" in file \"" +
       std::string(file_name()) + "\" line " + std::to_string(line()))
          .c_str();
#endif
}
