#pragma once

#if __has_include(<source_location>)
#  include <source_location>
#  define have_source_location 1
#elif __has_include(<experimental/source_location>)
#  include <experimental/source_location>
#  define have_source_location 1
#  define experimental_have_source_location 1
#else
#  define have_source_location 0
#endif
#include <exception>
#include <cstdint>
#include <string>

class Error : public std::exception
{
public:
#if experimental_have_source_location == 1
    Error(const int& code=0,const std::string& message="",std::experimental::source_location loc=std::experimental::source_location::current());
#elif have_source_location == 1
    Error(const int& code=0,const  std::string& message="",std::source_location loc=std::source_location::current());
#else
    Error(const int& code=0,const  std::string& message="");
#endif
    virtual ~Error(){};
    virtual const char* what() const noexcept;
#if have_source_location == 1
    const std::uint_least32_t line() const {return location.line();}
    const std::uint_least32_t column() const {return location.column();}
    const char* file_name() const {return location.file_name();}
    const char* function_name() const {return location.function_name();}
#endif
    const std::int_least32_t code() const {return m_code;}
    void setLoggerName(const std::string& name);
private:
    Error()=delete;
    void construct_message();
protected:
    std::string m_message{"Compile with C++20 for better informations !"};
    const std::int_least32_t m_code{0};
    virtual const char* ErrorStrings(const std::int_least32_t& code);
#if experimental_have_source_location == 1
    std::experimental::source_location location;
#elif have_source_location == 1
    const std::source_location& location;
#endif
};
