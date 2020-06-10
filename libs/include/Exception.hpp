#pragma once

#include "Internal.hpp"

#if __has_include(<source_location>)
  #include <source_location>
  #define have_source_location 1
#elif __has_include(<experimental/source_location>)
  #include <experimental/source_location>
  #define experimental_have_source_location 1
#else
  #define have_source_location 0
#endif

#include <exception>
#include <string>

enum class StatusCode : std::int_least32_t;

class Exception: public std::exception
{
public:
#if experimental_have_source_location == 1
  Exception(const StatusCode& statusCode, const std::string& message, std::experimental::source_location location = std::experimental::source_location::current());
#elif have_source_location == 1
  Exception(const StatusCode& statusCode, const std::string& message, std::source_location location = std::source_location::current());
#else
  Exception(const StatusCode& statusCode, const std::string& message);
#endif
  Exception(const Exception& e) = default;
  /** Get back trace at point of exception construction (gcc only)
       */
  const std::string& getBackTrace() const;
  virtual ~Exception() noexcept override;
  virtual const char* what() const noexcept override;
#if have_source_location == 1 || experimental_have_source_location ==1 
  const uint_least32_t getLine() const;
  const uint_least32_t getColumn() const;
  const char*          getFileName() const;
  const char*          getFunctionName() const;
#endif
  const int getCode() const;

protected:
#if experimental_have_source_location == 1
  Exception(const int& code, const std::string& message, std::experimental::source_location location = std::experimental::source_location::current());
#elif have_source_location == 1
  Exception(const int& code, const std::string& message, std::source_location location = std::source_location::current());
#else
  Exception(const int& code, const std::string& message);
#endif
  virtual const char* errorStrings(const int_least32_t& code);

private:
  
  void          createBackTrace();
  std::string   m_Message{"Compile with source_location support for better informations !"};
  const int_least32_t m_Code{0};
#if experimental_have_source_location == 1
  std::experimental::source_location m_Location;
#elif have_source_location == 1
  std::source_location m_Location;
#endif
  Exception() = delete;
  void        constructMessage();
};
