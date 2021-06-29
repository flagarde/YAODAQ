#include "Message2.hpp"
#include "Infos.hpp"

#include <iostream>
#include <iomanip>
#include <chrono>

#include <fmt/chrono.h>
#include <nlohmann/json.hpp>

#include "magic_enum.hpp"

namespace yaodaq
{

Message2::Message2()
{
  m_Json["from"] = "";
  m_Json["type"] = static_cast<std::string>(magic_enum::enum_name(TYPE::Unknown));
  m_Json["to"] = nlohmann::json::array();
  m_Json["timestamp"] = getISOCurrentTimestamp<std::chrono::milliseconds>();
}

void Message2::addTo(const Infos& info)
{
  m_Json["to"].push_back(nlohmann::json::parse(info.getJson()));
}

Message2::Message2(const std::string& str)
{
  m_Json = nlohmann::json::parse(str);
}

const nlohmann::json& Message2::get() const
{
  return m_Json;
}

Message2::Message2(const TYPE& type, const nlohmann::json& content)  : Message2()
{
  m_Json["content"] = content;
  m_Json["type"] = static_cast<std::string>(magic_enum::enum_name(type));
}

std::string Message2::dump(const int indent,
              const char indent_char,
              const bool ensure_ascii,
              const nlohmann::detail::error_handler_t error_handler) const
{
  return m_Json.dump(indent,indent_char,ensure_ascii,error_handler);
}

const nlohmann::json& Message2::geType() const
{
  return m_Json["type"];
}

const std::string Message2::getTypeStr() const
{
  return m_Json["type"].get<std::string>();
}

const TYPE Message2::getTYPE() const
{
  return magic_enum::enum_cast<TYPE>(getTypeStr()).value();
}

const nlohmann::json& Message2::getFrom() const
{
  return m_Json["from"];
}

const std::string Message2::getFromStr() const
{
  return m_Json["from"].get<std::string>();
}

const nlohmann::json& Message2::getTo() const
{
  return m_Json["to"];
}

const std::string Message2::getToStr() const
{
  return m_Json["to"].dump();
}

const nlohmann::json& Message2::getContent() const
{
  return m_Json["content"];
}

const std::string Message2::getContentStr() const
{
  if(m_Json["content"].is_string()) return m_Json["content"].get<std::string>();
  else if (m_Json["content"].dump() == "") return "";
  else return m_Json["content"].dump();
}

const nlohmann::json& Message2::getTimestamp() const
{
  return m_Json["timestamp"];
}

const std::string Message2::getTimestampStr() const
{
  return m_Json["timestamp"].get<std::string>();
}

bool Message2::empty()
{
  return getContentStr().empty();
}

};

std::ostream& operator<<(std::ostream& os, const yaodaq::Message2& message)
{
  return os<<message.dump(2);
}
