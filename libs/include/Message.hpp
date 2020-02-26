#pragma once

#include "json.h"

#include <memory>
#include <set>

enum class Type
{
  Trace,
  Info,
  Debug,
  Warning,
  Critical,
  Error,
  Status,
  Command
};

class Message
{
public:
  Message(const Type& type = Type::Info, const std::string& content = "", const std::string& to = "", const std::string& from = "");
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
  void         setType(const Type&);
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
  Command(const std::string& content = "", const std::string& to = "", const std::string& from = "");
};

class Critical: public Message
{
public:
  Critical(const std::string& content = "", const std::string& to = "", const std::string& from = "");
};

class Warning: public Message
{
public:
  Warning(const std::string& content = "", const std::string& to = "", const std::string& from = "");
};

class Debug: public Message
{
public:
  Debug(const std::string& content = "", const std::string& to = "", const std::string& from = "");
};

class Trace: public Message
{
public:
  Trace(const std::string& content = "", const std::string& to = "", const std::string& from = "");
};

class Error: public Message
{
public:
  Error(const std::string& content = "", const std::string& to = "", const std::string& from = "");
};

class Info: public Message
{
public:
  Info(const std::string& content = "", const std::string& to = "", const std::string& from = "");
};

class Status: public Message
{
public:
  Status(const std::string& content = "", const std::string& to = "", const std::string& from = "");
  virtual void setContent(const std::string&) final;

private:
  static std::set<std::string> m_Status;
  std::string                  checkContent(const std::string& content);
};
