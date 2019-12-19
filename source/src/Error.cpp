#include"Error.hpp"

#if experimental_have_source_location == 1
Error::Error(const int& code,const char* message,std::experimental::source_location loc):m_code(code),m_message(message),location(loc)
{
    construct_message();
}
#elif have_source_location == 1
Error::Error(const int& code) :m_code(code),location(std::source_location::current())
{
    construct_message();
}
#else
Error::Error(const int& code) :m_code(code)
{
    construct_message();
}
#endif
const char* Error::what() const noexcept
{
    return m_message.c_str();
}
