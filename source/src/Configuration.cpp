#include "Configuration.hpp"
#include "parser.hpp"
#include <iostream>

void Configuration::parse()
{
  m_Conf=toml::parse(m_Filename);
}

std::string Configuration::getFileName()
{
  return m_Filename;
}

void Configuration::setFileName(const std::string& file)
{
  m_Filename=file;
}

toml::value Configuration::operator()()
{
  return m_Conf;
}

