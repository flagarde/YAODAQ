#ifndef MESSAGE_HPP
#define MESSAGE_HPP
#include <memory>
#include "json.h"

class Message 
{
public:
  Message(const std::string& type ="Message",const std::string& from="Unknown",const std::string& to="",const std::string& content="");
  void setFrom(const std::string&);
  void setTo(const std::string&);
  void setContent(const std::string&);
  std::string getFrom() const;
  std::string getTo() const;
  std::string getContent() const;
  std::string get() const;
  std::string getType() const;
  std::string print() const;
  std::string getFrom();
  std::string getTo();
  std::string getContent();
  std::string get();
  std::string getType();
  std::string print();
protected:
  void setType(const std::string&);
private:
  static Json::StreamWriterBuilder m_Builder;
  std::unique_ptr<Json::StreamWriter> m_Writer{nullptr};
  Json::Value m_Value{};
};
#endif
