#pragma once

#include <iosfwd>

#include "States.hpp"
#include "LogLevels.hpp"
#include "Exception.hpp"


#include "Actions.hpp"

#include "json/value.h"
#include "json/reader.h"
#include "json/writer.h"

#include "fmt/format.h"

#include <memory>
#include <set>
#include <iostream>

namespace yaodaq
{


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

class Message
{
public:
  explicit Message(const TYPE& type = TYPE::Log, const std::string& content = "", const std::string& to = "ALL");
  explicit Message(const TYPE& type, const Json::Value& content, const std::string& to = "ALL");
  void        parse(const std::string&);
  void        setFrom(const std::string&);
  void        setTo(const std::string&);
  template<typename T> void addKey(const std::string& key, const T& value)
  {
    m_Value["Content"][key] = value;
  }
  void        setContent(const std::string&);
  void        setContent(const Json::Value&);
  std::string getFrom();

  std::string getFromStr() const;

  std::string getTo();
  std::string getContent();
  std::string getContent() const;
  std::string get();
  std::string get() const;
  TYPE getType() const;

  std::string getTypeStr() const;
  std::string getToStr() const;


  std::string getContentStr() const;


  std::string getStyled(const std::string& indent = "\t") const;
  void        setType(const TYPE&);
  bool        isEmpty();
  std::size_t getContentSize() const;
  Json::Value getContentAsJson() const;
  Json::Value getContentAsJson();

protected:
  Json::Value m_Value;
  std::unique_ptr<Json::StreamWriter> m_Writer{nullptr};
  std::unique_ptr<Json::CharReader>   m_Reader{nullptr};
private:
  static Json::StreamWriterBuilder    m_StreamWriterBuilder;
  static Json::CharReaderBuilder      m_CharReaderBuilder;
};

class Command: public Message
{
public:
  Command(const std::string& content = "", const std::string& to = "ALL");
  Command(const Message&);
};

// Log
class Log: public Message
{
public:
  Log(const LEVEL& level,const std::string& log,const std::string& to = "ALL");
  Log(const std::string& log,const std::string& to = "ALL");
  LEVEL getLevel() const;
  std::string getLevelStr() const;
  std::uint_least8_t getLevelUInt() const;
  std::string getLogStr() const;
  Log(const Message&);
};

// Open
class Open: public Message
{
public:
  Open(const std::string& uri,const std::map<std::string,std::string>& headers,const std::string& protocol,const std::string& to = "ALL");
  std::string getURIStr() const;
  std::map<std::string,std::string> getHeaders() const;
  std::string getProtocolStr() const;
  std::string getKeyStr() const;
  std::string getIdStr() const;
  Open(const Message&);
};

// EXCEPTION
class Except: public Message
{
public:
  Except(const Exception& exception, const std::string& to = "ALL");
  std::string getWhatStr() const;
  std::string getDescriptionStr() const;
  std::string getCodeStr() const;
  std::int_least32_t getCode() const;
  std::string getLineStr() const;
  std::int_least32_t getLine() const;
  std::string getColumnStr() const;
  std::int_least32_t getColumn() const;
  std::string getFileName() const;
  std::string getFunctionName() const;
  Except(const Message&);
};

// CLOSE
class Close : public Message
{
public:
  Close(const std::uint16_t& code,const std::string& reason,const bool& remote,const std::string& to = "ALL");
  std::uint16_t getCode() const;
  std::string getCodeStr() const;
  std::string getReasonStr() const;
  bool getRemote() const;
  std::string getRemoteStr() const;
  Close(const Message&);
};

// CONNECTOR_ERROR
class ConnectionError : public Message
{
public:
  ConnectionError(const std::uint32_t& retries, const double& wait_time, const int& http_status, const std::string& reason, const bool& decompressionError,const std::string& to = "ALL");
  std::uint32_t getRetries() const;
  double getWaitTime() const;
  int getHTTPStatus() const;
  std::string getReason() const;
  bool getCompressionError() const;
  ConnectionError(const Message&);
};

class Ping : public Message
{
public:
  Ping(const std::string& content, const std::string& to = "ALL");
  Ping(const Message&);
};

class Pong : public Message
{
public:
  Pong(const std::string& content, const std::string& to = "ALL");
  Pong(const Message&);
};

// STATE
class State: public Message
{
public:
  State(const STATE& state, const std::string& to = "ALL");
  STATE getState() const;
  std::string getStateStr() const;
  State(const Message&);
};

// ACTION
class Action: public Message
{
public:
  Action(const ACTION& action, const std::string& to = "ALL");
  ACTION getAction() const;
  std::string getActionStr() const;
  Action(const Message&);
};

class Data: public Message
{
public:
  Data(const std::string& content, const std::string& to = "ALL");
  Data(const Json::Value& content = {}, const std::string& to = "ALL");
  Data(const Message&);
};

class Response: public Message
{
public:
  Response(const std::string& content = "", const std::string& to = "ALL");
  Response(const Message&);
};

class Unknown: public Message
{
public:
  Unknown(const std::string& content = "", const std::string& to = "ALL");
  Unknown(const Message&);
};


};

std::ostream& operator<<(std::ostream& os, const yaodaq::Message& message);
