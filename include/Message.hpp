#pragma once

#include "States.hpp"

#include "json/value.h"
#include "json/reader.h"
#include "json/writer.h"

#include <memory>
#include <set>

enum class Types
{
  Trace,
  Info,
  Debug,
  Warning,
  Critical,
  Error,
  State,
  Action,
  Command,
  Response,
  Data,
  Unknow,
};

enum class Actions
{
  INITIALIZE,
  CONNECT,
  CONFIGURE,
  START,
  PAUSE,
  STOP,
  CLEAR,
  DISCONNECT,
  RELEASE,
  QUIT
};

class Message
{
public:
  explicit Message(const Types& type = Types::Info, const std::string& content = "", const std::string& to = "ALL", const std::string& from = "");
  explicit Message(const Types& type, const Json::Value& content, const std::string& to = "ALL", const std::string& from = "");
  void        parse(const std::string&);
  void        setFrom(const std::string&);
  void        setTo(const std::string&);
  void        addKey(const std::string& key, const std::string& value);
  void        setContent(const std::string&);
  void        setContent(const Json::Value&);
  std::string getFrom();
  std::string getTo();
  std::string getContent();
  std::string getContent() const;
  std::string get();
  std::string get() const;
  std::string getType();
  std::string getStyled(const std::string& indent = "\t");
  void        setType(const Types&);
  bool        isEmpty();
  std::size_t getContentSize();
  Json::Value getContentAsJson() const;
  Json::Value getContentAsJson();

protected:
  Json::Value m_Value{};

private:
  static Json::StreamWriterBuilder    m_StreamWriterBuilder;
  std::unique_ptr<Json::StreamWriter> m_Writer{nullptr};
  std::unique_ptr<Json::CharReader>   m_Reader{nullptr};
  static Json::CharReaderBuilder      m_CharReaderBuilder;
};

class Command: public Message
{
public:
  Command(const std::string& content = "", const std::string& to = "ALL", const std::string& from = "");
  std::string               getCommand() const;
  std::string               getCommand();
  template<typename T> void addParameter(const std::string& name, const T& value) { m_Value["Content"]["Parameters"][name] = value; }
  Json::Value               getParameter(const std::string& parameter) const
  {
    if(m_Value["Content"]["Parameters"].isMember(parameter)) return m_Value["Content"]["Parameters"][parameter];
    else
      return Json::Value{"ERROR"};
  }
};

class Critical: public Message
{
public:
  Critical(const std::string& content = "", const std::string& to = "ALL", const std::string& from = "");
};

class Warning: public Message
{
public:
  Warning(const std::string& content = "", const std::string& to = "ALL", const std::string& from = "");
};

class Debug: public Message
{
public:
  Debug(const std::string& content = "", const std::string& to = "ALL", const std::string& from = "");
};

class Trace: public Message
{
public:
  Trace(const std::string& content = "", const std::string& to = "ALL", const std::string& from = "");
};

class Error: public Message
{
public:
  Error(const std::string& content = "", const std::string& to = "ALL", const std::string& from = "");
};

class Info: public Message
{
public:
  Info(const std::string& content = "", const std::string& to = "ALL", const std::string& from = "");
};

class State: public Message
{
public:
  State(States state, const std::string& to = "ALL", const std::string& from = "");
};

class Action: public Message
{
public:
  Action(const Actions& action, const std::string& to = "ALL", const std::string& from = "");
};

class Data: public Message
{
public:
  Data(const std::string& content, const std::string& to = "ALL", const std::string& from = "");
  Data(const Json::Value& content = {}, const std::string& to = "ALL", const std::string& from = "");
};

class Response: public Message
{
public:
  Response(const std::string& content = "", const std::string& to = "ALL", const std::string& from = "");
};
