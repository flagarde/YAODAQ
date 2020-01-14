#include "Message.hpp"
#include "json.h"

Json::StreamWriterBuilder Message::m_Builder= Json::StreamWriterBuilder();

Message::Message(const std::string& type,const std::string& from,const std::string& to,const std::string& content)
{
  m_Writer.reset(m_Builder.newStreamWriter());
  setType(type);
  setFrom(from);
  setTo(to);
  setContent(content);
}

void Message::setFrom(const std::string& from)
{
  m_Value["From"] =from;
}

void Message::setTo(const std::string& to)
{
  m_Value["To"] =to;
}

void Message::setType(const std::string& type)
{
  m_Value["Type"] =type;
}

void Message::setContent(const std::string& content)
{
  m_Value["Content"] =content;
}

std::string Message::get() const
{
  return get();
}


std::string Message::print() const
{
  return print();
}

std::string Message::get() 
{
  m_Builder.settings_["indentation"] = "";
  return Json::writeString(m_Builder,m_Value);
}


std::string Message::print()
{
  m_Builder.settings_["indentation"] = "\t";
  return Json::writeString(m_Builder,m_Value);
}


std::string Message::getFrom()
{
  return m_Value["From"].asString();
}

std::string Message::getTo()
{
  return m_Value["To"].asString();
}

std::string Message::getContent()
{
  return m_Value["Content"].asString();
}

std::string Message::getType()
{
  return m_Value["Type"].asString();
}

std::string Message::getFrom() const
{
  return getFrom();
}

std::string Message::getTo() const
{
  return getTo();
}

std::string Message::getContent() const
{
  return getContent();
}

std::string Message::getType() const
{
  return getType();
}
