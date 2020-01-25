#ifndef CONNECTOR_HPP
#define CONNECTOR_HPP
#include <string>
#include "value.hpp"

class Connector
{
public:
  Connector(const std::string& type="",const std::string& name="");
  virtual std::int32_t Connect()=0;
  virtual void Disconnect()=0;
  virtual ~Connector(){};
  void setConfiguration(const toml::value& config);
  void printParameters();
protected:
  std::string m_Type{""};
  std::string m_Name{""};
  std::int32_t m_Handle{0};
  toml::value m_Configs;
  virtual void verifyConfig();
};

#endif
