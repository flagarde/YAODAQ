#include "Message.hpp"

#include "Exception.hpp"
#include "json.h"

#include <iostream>

Json::StreamWriterBuilder Message::m_StreamWriterBuilder =
    Json::StreamWriterBuilder();

Json::CharReaderBuilder Message::m_CharReaderBuilder =
    Json::CharReaderBuilder();

Message::Message(const std::string& type, const std::string& content,
                 const std::string& to, const std::string& from)
{
  m_Writer.reset(m_StreamWriterBuilder.newStreamWriter());
  m_Reader.reset(m_CharReaderBuilder.newCharReader());
  setType(type);
  setFrom(from);
  setTo(to);
  setContent(content);
}

void Message::setFrom(const std::string& from)
{
  m_Value["From"] = from;
}

void Message::setTo(const std::string& to)
{
  m_Value["To"] = to;
}

void Message::setType(const std::string& type)
{
  m_Value["Type"] = type;
}

void Message::setContent(const std::string& content)
{
  m_Value["Content"] = content;
}

std::string Message::get() const
{
  return get();
}

void Message::parse(const std::string& msg)
{
  Json::String errs;
  bool         ok = m_Reader->parse(&msg[0], &msg[msg.size()], &m_Value, &errs);
  if(!ok) { throw Exception(ok, "Error: " + errs); }
}

std::string Message::print() const
{
  return print();
}

std::string Message::get()
{
  m_StreamWriterBuilder.settings_["indentation"] = "";
  return Json::writeString(m_StreamWriterBuilder, m_Value);
}

std::string Message::getStyled(const std::string& indent)
{
  m_StreamWriterBuilder.settings_["indentation"] = indent;
  return Json::writeString(m_StreamWriterBuilder, m_Value);
}

void Message::print(const std::string& indent)
{
  m_StreamWriterBuilder.settings_["indentation"] = indent;
  std::cout << Json::writeString(m_StreamWriterBuilder, m_Value) << std::endl;
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

Info::Info(const std::string& content, const std::string& to,
           const std::string& from)
    : Message("Info", content, to, from)
{
}

std::set<std::string> Status::m_Status{
    "UNINITIALIZED", "INITIALIZED", "CONNECTED", "CONFIGURED",   "STARTED",
    "PAUSED",        "STOPED",      "CLEARED",   "DISCONNECTED", "RELEASED",
    "QUITED",        "INITIALIZE",  "CONNECT",   "CONFIGURE",    "START",
    "PAUSE",         "STOP",        "CLEAR",     "DISCONNECT",   "RELEASE",
    "QUIT"};

Status::Status(const std::string& content, const std::string& to,
               const std::string& from)
    : Message("Status", checkContent(content), to, from)
{
}

std::string Status::checkContent(const std::string& content)
{
  if(m_Status.find(content) == m_Status.end()) return "UNKNOWN_STATUS";
  else
    return content;
}

void Status::setContent(const std::string& content)
{
  Message::setContent(checkContent(content));
}

Log::Log(const std::string& content, const std::string& to,
         const std::string& from)
    : Message("Log", content, to, from)
{
}
