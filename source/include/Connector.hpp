#ifndef CONNECTOR_HPP
#define CONNECTOR_HPP

class Connector
{
public:
  virtual void Connect()=0;
  virtual void Disconnect()=0;
  virtual ~Connector(){};
private:
  std::string m_Type{""};
  std::string m_Name{""};
};

#endif
