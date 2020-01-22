#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP
#include <string>
#include "parser.hpp"
#include "literal.hpp"
#include "serializer.hpp"
#include "get.hpp"

class Configuration 
{
public:
  void parse();
  std::string getFileName();
  void setFileName(const std::string&);
  toml::value operator()();
private:
  std::string m_Filename{""};
  toml::value m_Conf;
};

#endif
