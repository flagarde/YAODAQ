#ifndef INFO_HPP
#define INFO_HPP
#include <string>
#include "Message.hpp"

class Info : public Message
{
public:
  Info(const std::string& content="",const std::string& to="",const std::string& from="");
};

#endif
