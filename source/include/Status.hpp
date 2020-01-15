#ifndef STATUS_HPP
#define STATUS_HPP
#include <string>
#include <set>
#include "Message.hpp"

class Status : public Message
{
public:
  Status(const std::string& content="",const std::string& to="",const std::string& from="");
  virtual void setContent(const std::string&) final;
private:
  static std::set<std::string> m_Status;
  std::string checkContent(const std::string& content);
};
 
#endif
