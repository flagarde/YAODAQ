#pragma once

#include "States.hpp"
#include "json.h"

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
  Command
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
  Message(const Types& type = Types::Info, const std::string& content = "", const std::string& to = "ALL", const std::string& from = "");
  void         parse(const std::string&);
  void         setFrom(const std::string&);
  void         setTo(const std::string&);
  virtual void setContent(const std::string&);
  std::string  getFrom() const;
  std::string  getTo() const;
  std::string  getContent() const;
  std::string  get() const;
  std::string  print() const;
  std::string  getFrom();
  std::string  getTo();
  std::string  getContent();
  std::string  get();
  std::string  getType();
  std::string  getType() const;
  std::string  getStyled(const std::string& indent = "\t");
  void         print(const std::string& indent = "\t");
  void         setType(const Types&);
  bool         isEmpty();

private:
  static Json::StreamWriterBuilder    m_StreamWriterBuilder;
  std::unique_ptr<Json::StreamWriter> m_Writer{nullptr};
  std::unique_ptr<Json::CharReader>   m_Reader{nullptr};
  static Json::CharReaderBuilder      m_CharReaderBuilder;
  Json::Value                         m_Value{};
};

class Command: public Message
{
public:
  Command(const std::string& content = "", const std::string& to = "ALL", const std::string& from = "");
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
  State(const States& state, const std::string& to = "ALL", const std::string& from = "");
};

class Action: public Message
{
public:
  Action(const Actions& action, const std::string& to = "ALL", const std::string& from = "");
};
