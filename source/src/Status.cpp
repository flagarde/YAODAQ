#include "Status.hpp"
 
std::set<std::string> Status::m_Status{"UNINITIALIZED","INITIALIZED","CONNECTED","CONFIGURED","STARTED","PAUSED","STOPED","CLEARED","DISCONNECTED","RELEASED"};
 
Status::Status(const std::string& content,const std::string& to,const std::string& from):Message("Status",checkContent(content),to,from)
{

}

std::string Status::checkContent(const std::string& content)
{
  if(m_Status.find(content)==m_Status.end()) return "UNKNOWN_STATUS";
  else return content;
}

void Status::setContent(const std::string& content)
{
  Message::setContent(checkContent(content));
}
