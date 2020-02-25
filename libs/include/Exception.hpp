#pragma once

#if __has_include(<source_location>)
  #include <source_location>
  #define have_source_location 1
#elif __has_include(<experimental/source_location>)
  #include <experimental/source_location>
  #define have_source_location              1
  #define experimental_have_source_location 1
#else
  #define have_source_location 0
#endif
#include <cstdint>
#include <exception>
#include <string>

enum StatusCode
{
  STATUS_CODE_SUCCESS,
  STATUS_CODE_FAILURE,
  STATUS_CODE_NOT_FOUND,
  STATUS_CODE_NOT_INITIALIZED,
  STATUS_CODE_ALREADY_INITIALIZED,
  STATUS_CODE_ALREADY_PRESENT,
  STATUS_CODE_OUT_OF_RANGE,
  STATUS_CODE_NOT_ALLOWED,
  STATUS_CODE_INVALID_PARAMETER,
  STATUS_CODE_UNCHANGED,
  STATUS_CODE_INVALID_PTR,
  STATUS_CODE_JSON_PARSING,
  STATUS_CODE_WRONG_NUMBER_PARAMETERS
};

class Exception: public std::exception
{
public:
#if experimental_have_source_location == 1
  Exception(const StatusCode statusCode, const std::string& message = "",
            std::experimental::source_location location = std::experimental::source_location::current());
#elif have_source_location == 1
  Exception(const StatusCode statusCode, const std::string& message = "", std::source_location location = std::source_location::current());
#else
  Exception(const StatusCode statusCode, const std::string& message = "");
#endif
  Exception(const Exception& e) = default;
  /** Get back trace at point of exception construction (gcc only)
       */
  const std::string& getBackTrace() const;
  virtual ~Exception() override;
  virtual const char* what() const noexcept;
#if have_source_location == 1
  const std::uint_least32_t getLine() const;
  const std::uint_least32_t getColumn() const;
  const char*               getFileName() const;
  const char*               getFunctionName() const;
#endif
  const int getCode() const;
  /** Get status code as a string
  */
  virtual std::string toString() const;

protected:
#if experimental_have_source_location == 1
  Exception(const int& code = 0, const std::string& message = "",
            std::experimental::source_location location = std::experimental::source_location::current());
#elif have_source_location == 1
  Exception(const int& code = 0, const std::string& message = "", std::source_location location = std::source_location::current());
#else
  Exception(const int& code = 0, const std::string& message = "");
#endif
  virtual const char* errorStrings(const std::int_least32_t& code);

private:
  void        createBackTrace();
  std::string m_Message{"Compile with C++20 for better informations !"};
  const int   m_Code{STATUS_CODE_SUCCESS};
#if experimental_have_source_location == 1
  std::experimental::source_location m_Location;
#elif have_source_location == 1
  std::source_location m_Location;
#endif
  Exception() = delete;
  void        constructMessage();
  std::string m_backTrace = {""};
};
