#pragma once

#include <string>
#include <iosfwd>

#include <nlohmann/json.hpp>
#include <date/date.h>

namespace yaodaq
{

class Infos;

  enum class TYPE
{
  //IXWebSocket MessageType
  // Message is not set here
  Open,
  Close,
  ConnectionError,
  Ping,
  Pong,
  Fragment,

  Log,
  Exception,

  State,
  Action,
  Command,
  Response,
  Data,
  Unknown,
};

class Message2
{
public:
  std::string dump(const int indent = -1,
              const char indent_char = ' ',
              const bool ensure_ascii = false,
              const nlohmann::detail::error_handler_t error_handler = nlohmann::detail::error_handler_t::strict) const;
  explicit Message2(const TYPE&, const nlohmann::json&);
  explicit Message2(const std::string&);
  const nlohmann::json& get() const;
  const nlohmann::json& getFrom() const;
  const std::string getFromStr() const;
  const nlohmann::json& geType() const;
  const std::string getTypeStr() const;
  const TYPE getTYPE() const;
  const nlohmann::json& getTo() const;
  const std::string getToStr() const;
  const nlohmann::json& getContent() const;
  const std::string getContentStr() const;
  const nlohmann::json& getTimestamp() const;
  const std::string getTimestampStr() const;
  bool empty();
  void addTo(const Infos&);
protected:

private:
  explicit Message2();
  template <class Precision> std::string getISOCurrentTimestamp()
  {
    return date::format("%FT%TZ", date::floor<Precision>(std::chrono::system_clock::now()));
  }
  nlohmann::json m_Json;
};

};

std::ostream& operator<<(std::ostream& os, const yaodaq::Message2& message);
