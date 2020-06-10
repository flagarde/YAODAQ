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

enum class StatusCode : int_least32_t;

class Exception: public std::exception
{
public:
#if experimental_have_source_location == 1
  Exception(const StatusCode& statusCode, const std::string& description, const std::experimental::source_location& location = std::experimental::source_location::current());
#elif have_source_location == 1
  Exception(const StatusCode& statusCode, const std::string& description, const std::source_location& location = std::source_location::current());
#else
  Exception(const StatusCode& statusCode, const std::string& description);
#endif
  Exception(const Exception& e) = default;
  virtual ~Exception() noexcept override;
  virtual const char* what() const noexcept override;
#if have_source_location == 1 || experimental_have_source_location == 1
  const uint_least32_t getLine() const;
  const uint_least32_t getColumn() const;
  const char*          getFileName() const;
  const char*          getFunctionName() const;
  const char*          getDescription() const;
#endif
  const int_least32_t getCode() const;

protected:
#if experimental_have_source_location == 1
  Exception(const int_least32_t& code, const std::string& description, const std::experimental::source_location& location = std::experimental::source_location::current());
#elif have_source_location == 1
  Exception(const int_least32_t& code, const std::string& description, const std::source_location& location = std::source_location::current());
#else
  Exception(const int_least32_t& code, const std::string& description);
#endif

private:
  std::string         m_Description{""};
  std::string         m_Message{""};
  const int_least32_t m_Code{0};
  Exception() = delete;
  void constructMessage() noexcept;
#if experimental_have_source_location == 1
  std::experimental::source_location m_Location;
#elif have_source_location == 1
  std::source_location m_Location;
#endif
};
