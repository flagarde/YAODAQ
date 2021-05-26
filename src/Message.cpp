#include "Message.hpp"

#include "Exception.hpp"
#include "StatusCode.hpp"
#include "json/json.h"
#include "magic_enum.hpp"

Json::StreamWriterBuilder Message::m_StreamWriterBuilder = Json::StreamWriterBuilder();

Json::CharReaderBuilder Message::m_CharReaderBuilder = Json::CharReaderBuilder();

void Message::addKey(const std::string& key, const std::string& value)
{
  m_Value["Content"][key] = value;
}

std::size_t Message::getContentSize()
{
  return getContent().size();
}

Message::Message(const Types& type, const std::string& content, const std::string& to)
{
  m_Writer.reset(m_StreamWriterBuilder.newStreamWriter());
  m_Reader.reset(m_CharReaderBuilder.newCharReader());
  setType(type);
  setTo(to);
  if(content != "") setContent(content);
}

Message::Message(const Types& type, const Json::Value& content, const std::string& to)
{
  m_Writer.reset(m_StreamWriterBuilder.newStreamWriter());
  m_Reader.reset(m_CharReaderBuilder.newCharReader());
  setType(type);
  setTo(to);
  setContent(content);
}

bool Message::isEmpty()
{
  if(getContent() == "") return true;
  else
    return false;
}

void Message::setFrom(const std::string& from)
{
  m_Value["From"] = from;
}

void Message::setTo(const std::string& to)
{
  m_Value["To"] = to;
}

void Message::setType(const Types& type)
{
  m_Value["Type"] = std::string(magic_enum::enum_name(type));
}

void Message::setContent(const std::string& content)
{
  m_Value["Content"] = content;
}

void Message::setContent(const Json::Value& content)
{
  m_Value["Content"] = content;
}

void Message::parse(const std::string& msg)
{
  Json::String errs;
  bool         ok = m_Reader->parse(&msg[0], &msg[msg.size()], &m_Value, &errs);
  if(!ok) { throw Exception(StatusCode::JSON_PARSING, errs); }
}

std::string Message::get()
{
  m_StreamWriterBuilder.settings_["indentation"] = "";
  return Json::writeString(m_StreamWriterBuilder, m_Value);
}

std::string Message::get() const
{
  m_StreamWriterBuilder.settings_["indentation"] = "";
  return Json::writeString(m_StreamWriterBuilder, m_Value);
}

std::string Message::getStyled(const std::string& indent)
{
  m_StreamWriterBuilder.settings_["indentation"] = indent;
  return Json::writeString(m_StreamWriterBuilder, m_Value);
}

Json::Value Message::getContentAsJson() const
{
  return m_Value["Content"];
}

Json::Value Message::getContentAsJson()
{
  return m_Value["Content"];
}

std::string Message::getFrom()
{
  m_StreamWriterBuilder.settings_["indentation"] = "";
  return Json::writeString(m_StreamWriterBuilder, m_Value["From"]);
}

std::string Message::getTo()
{
  m_StreamWriterBuilder.settings_["indentation"] = "";
  return Json::writeString(m_StreamWriterBuilder, m_Value["To"]);
}

std::string Message::getContent()
{
  if(m_Value["Content"].isString()) return m_Value["Content"].asString();
  else
  {
    m_StreamWriterBuilder.settings_["indentation"] = "";
    return Json::writeString(m_StreamWriterBuilder, m_Value["Content"]);
  }
}

std::string Message::getContent() const
{
  if(m_Value["Content"].isString()) return m_Value["Content"].asString();
  else
  {
    m_StreamWriterBuilder.settings_["indentation"] = "";
    return Json::writeString(m_StreamWriterBuilder, m_Value["Content"]);
  }
}

std::string Message::getType()
{
  return m_Value["Type"].asString();
}

Info::Info(const std::string& content, const std::string& to): Message(Types::Info, content, to) {}

State::State(States state, const std::string& to): Message(Types::State, std::string(magic_enum::enum_name(state)), to) {}

Action::Action(const Actions& action, const std::string& to): Message(Types::Action, std::string(magic_enum::enum_name(action)), to) {}

Error::Error(const std::string& content, const std::string& to): Message(Types::Error, content, to) {}

Trace::Trace(const std::string& content, const std::string& to): Message(Types::Trace, content, to) {}

Debug::Debug(const std::string& content, const std::string& to): Message(Types::Debug, content, to) {}

Warning::Warning(const std::string& content, const std::string& to): Message(Types::Warning, content, to) {}

Critical::Critical(const std::string& content, const std::string& to): Message(Types::Critical, content, to) {}

Command::Command(const std::string& content, const std::string& to): Message(Types::Command, std::string(""), to)
{
  m_Value["Content"]["Command"] = content;
}
std::string Command::getCommand()
{
  return m_Value["Content"]["Command"].asString();
}
std::string Command::getCommand() const
{
  return getCommand();
}

Data::Data(const std::string& content, const std::string& to): Message(Types::Data, content, to) {}
Data::Data(const Json::Value& content, const std::string& to): Message(Types::Data, content, to) {}
Response::Response(const std::string& content, const std::string& to): Message(Types::Response, content, to) {}
