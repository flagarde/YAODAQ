#ifndef ERROR_HPP
#define ERROR_HPP
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
#include<exception>
#include<cstdint>
#include<string>

class Error : public std::exception
{
public:
    Error(const int& code=0,const char* message="",std::experimental::source_location loc=std::experimental::source_location::current());
    virtual ~Error(){};
    virtual const char* what() const noexcept;
#if have_source_location == 1
    const std::uint_least32_t line() const {return location.line();}
    const std::uint_least32_t column() const {return location.column();}
    const char* file_name() const {return location.file_name();}
    const char* function_name() const {return location.function_name();}
#endif
    const std::int_least32_t code() const {return m_code;}
    
private:
    Error()=delete;
    void construct_message()
    {
        m_message="Error "+std::to_string(m_code)+"\n\t-> "+m_message;
#if have_source_location == 1
        m_message+=(" in function \""+std::string(function_name())+"\" in file \""+std::string(file_name())+"\" line "+std::to_string(line())).c_str();
#endif
    }
protected:
    std::string m_message{"Compile with C++20 for better informations !"};
    const std::int_least32_t m_code{0};
    virtual const char* ErrorStrings(const std::int_least32_t& code)=0;
#if experimental_have_source_location == 1
    std::experimental::source_location location;
#elif have_source_location == 1
    const std::source_location& location;
#endif
};
#endif
