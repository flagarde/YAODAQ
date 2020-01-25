#include "Connector.hpp"
#include <iostream>
#include "parser.hpp"
#include "literal.hpp"
#include "serializer.hpp"
#include "get.hpp"

Connector::Connector(const std::string& type,const std::string& name):m_Type(type),m_Name(name)
{
  
}

void Connector::setConfiguration(const toml::value& config)
{
  m_Configs=config;
  verifyConfig();
}

void Connector::verifyConfig()
{
  
}

void Connector::printParameters()
{
  std::cout<<m_Configs<<std::endl;
}
