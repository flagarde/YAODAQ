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

class Exception: public std::exception
{
public:
#if experimental_have_source_location == 1
  Exception(const int& code = 0, const std::string& message = "",
            std::experimental::source_location location =
                std::experimental::source_location::current());
#elif have_source_location == 1
  Exception(const int& code = 0, const std::string& message = "",
            std::source_location location = std::source_location::current());
#else
  Exception(const int& code = 0, const std::string& message = "");
#endif
  virtual ~Exception() override;
  virtual const char* what() const noexcept;
#if have_source_location == 1
  const std::uint_least32_t line() const;
  const std::uint_least32_t column() const;
  const char*               fileName() const;
  const char*               functionName() const;
#endif
  const std::int_least32_t code() const;

protected:
  virtual const char* errorStrings(const std::int_least32_t& code);

private:
  std::string m_Message{"Compile with C++20 for better informations !"};
  const std::int_least32_t m_Code{0};
#if experimental_have_source_location == 1
  std::experimental::source_location m_Location;
#elif have_source_location == 1
  std::source_location m_Location;
#endif
  Exception() = delete;
  void constructMessage();
};
