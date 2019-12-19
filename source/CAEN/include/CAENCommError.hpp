#ifndef CAENCOMMERROR_HPP
#define CAENCOMMERROR_HPP
#include "Error.hpp"
#include<cstdint>

namespace CAEN
{

class CAENCommError : public Error 
{
public:
#if experimental_have_source_location == 1
    CAENCommError(const int& code=0,std::experimental::source_location loc=std::experimental::source_location::current());
#elif have_source_location == 1
    CAENCommError(const int& code=0,std::source_location loc=std::source_location::current());
#else
    CAENCommError(const int& code=0)
#endif
private:
    CAENCommError()=delete;
    virtual const char* ErrorStrings(const std::int_least32_t& code) final;
};

}
#endif
