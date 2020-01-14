#ifndef INFOS_HPP
#define INFOS_HPP
#include <string>
#include "Message.hpp"

class Infos : public Message
{
public:
  Infos(const std::string& from="Unknown",const std::string& to="",const std::string& content="");
};

#endif
