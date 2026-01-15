#include "Message.hpp"

#include "Exception.hpp"
#include "StatusCode.hpp"
#include "json/json.h"
#include "magic_enum/magic_enum.hpp"

namespace yaodaq
{

//protected constructor to initialize the keys
/*Message::Message()
{
  m_Value["To"] = "";
  m_Value["From"] = "";
  m_Value["Content"] = Json::Value{};
}*/

Json::StreamWriterBuilder Message::m_StreamWriterBuilder = Json::StreamWriterBuilder();

Json::CharReaderBuilder Message::m_CharReaderBuilder = Json::CharReaderBuilder();

std::size_t Message::getContentSize() const
{
  return getContent().size();
}

Message::Message(const TYPE& type, const std::string& content, const std::string& to)
{
  m_Writer.reset(m_StreamWriterBuilder.newStreamWriter());
  m_Reader.reset(m_CharReaderBuilder.newCharReader());
  setType(type);
  setTo(to);
  if(content != "") setContent(content);
}

Message::Message(const TYPE& type, const Json::Value& content, const std::string& to)
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

Json::Value Message::getKey(const std::string& key) const
{
  return m_Value["Content"][key];
}


void Message::setType(const TYPE& type)
{
  m_Value["Type"] = static_cast<std::string>(magic_enum::enum_name(type));
}

void Message::setContent(const std::string& content)
{
  Json::String errs;
  bool         ok = m_Reader->parse(&content[0], &content[content.size()], &m_Value["Content"], &errs);
  if(!ok) { throw Exception(StatusCode::JSON_PARSING, errs); }
 // m_Value["Content"] = content;
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

std::string Message::getStyled(const std::string& indent) const
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
  return m_Value["From"].asString();
}

std::string Message::getFromStr() const
{
  return m_Value["From"].asString();
}


std::string Message::getToStr() const
{
  return m_Value["To"].asString();
}

std::string Message::getTo()
{
  return m_Value["To"].asString();
}

Message::Message(const Message& message)
{
  m_Value=message.m_Value;
}

std::string Message::getContentStr() const
{
  m_StreamWriterBuilder.settings_["indentation"] = "";
  return Json::writeString(m_StreamWriterBuilder, m_Value["Content"]);
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

TYPE Message::getType() const
{
  return magic_enum::enum_cast<TYPE>(getTypeStr()).value();
}

std::string Message::getTypeStr() const
{
  return m_Value["Type"].asString();
}

// Log
Log::Log(const LEVEL& level,const std::string& log,const std::string& to) : Message(TYPE::Log, Json::Value{},to)
{
  addKey("Log",log);
  addKey("Level",static_cast<std::string>(magic_enum::enum_name(level)));
}

Log::Log(const std::string& log,const std::string& to) : Log(LEVEL::Info, log, to)
{

}

LEVEL Log::getLevel() const
{
  return magic_enum::enum_cast<LEVEL>(getLevelStr()).value();
}

std::string Log::getLevelStr() const
{
  return m_Value["Content"]["Level"].asString();
}

std::uint_least8_t Log::getLevelUInt() const
{
  return static_cast<std::uint_least8_t>(getLevel());
}

std::string Log::getLogStr() const
{
  return m_Value["Content"]["Log"].asString();
}

Log::Log(const Message& message)
{
  setContent(message.getContentAsJson());
  setFrom(message.getFromStr());
  setTo(message.getToStr());
  if(message.getType()!=TYPE::Log) throw Exception(StatusCode::INVALID_CONVERSION,"Impossible to convert \"{}\" to \"{}\"",message.getTypeStr(),"Log");
  else setType(message.getType());
}

// Open
// Open
Open::Open(const std::string& uri,const std::map<std::string,std::string>& headers,const std::string& protocol,const std::string& to) : Message(TYPE::Open,Json::Value{}, to)
{
  addKey("URI",uri);
  addKey("Headers",Json::Value{});
  for(std::map<std::string,std::string>::const_iterator it=headers.begin();it!=headers.end();++it)
  {
    m_Value["Content"]["Headers"][it->first]=it->second;
  }
  addKey("Protocol",protocol);
}

std::string Open::getURIStr() const
{
  return m_Value["Content"]["URI"].asString();
}

std::map<std::string,std::string> Open::getHeaders() const
{
  std::map<std::string,std::string> ret;
  for(auto const& id : m_Value["Content"]["Headers"].getMemberNames())
  {
    ret[id]=m_Value["Content"]["Headers"][id].asString();
  }
  return std::move(ret);
}

std::string Open::getProtocolStr() const
{
  return m_Value["Content"]["Protocol"].asString();
}

Open::Open(const Message& message)
{
  setContent(message.getContentAsJson());
  setFrom(message.getFromStr());
  setTo(message.getToStr());
  if(message.getType()!=TYPE::Open) throw Exception(StatusCode::INVALID_CONVERSION,"Impossible to convert \"{}\" to \"{}\"",getTypeStr(),"Open");
  else setType(message.getType());
}

std::string Open::getKeyStr() const
{
  return m_Value["Content"]["Key"].asString();
}

std::string Open::getIdStr() const
{
  return m_Value["Content"]["ID"].asString();
}

// EXCEPT
Except::Except(const Exception& exception, const std::string& to) : Message(TYPE::Exception,Json::Value{}, to)
{
  addKey("Code",exception.code());
  addKey("Description",exception.description());
  addKey("Line",exception.line());
  addKey("Column",exception.column());
  addKey("File Name",exception.file_name());
  addKey("Function",exception.function_name());
  addKey("What",exception.what());
}

std::string Except::getWhatStr() const
{
  return m_Value["Content"]["What"].asString();
}

std::string Except::getDescriptionStr() const
{
  return m_Value["Content"]["Description"].asString();
}

std::string Except::getCodeStr() const
{
  return m_Value["Content"]["Code"].asString();
}

std::int_least32_t Except::getCode() const
{
  return m_Value["Content"]["Code"].asInt();
}

std::string Except::getLineStr() const
{
  return m_Value["Content"]["Line"].asString();
}

std::int_least32_t Except::getLine() const
{
  return m_Value["Content"]["Line"].asInt();
}

std::string Except::getColumnStr() const
{
  return m_Value["Content"]["Column"].asString();
}

std::int_least32_t Except::getColumn() const
{
  return m_Value["Content"]["Column"].asInt();
}

std::string Except::getFileName() const
{
  return m_Value["Content"]["File Name"].asString();
}

std::string Except::getFunctionName() const
{
  return m_Value["Content"]["Function Name"].asString();
}

Except::Except(const Message& message)
{
  setContent(message.getContentAsJson());
  setFrom(message.getFromStr());
  setTo(message.getToStr());
  if(message.getType()!=TYPE::Exception) throw Exception(StatusCode::INVALID_CONVERSION,"Impossible to convert \"{}\" to \"{}\"",getTypeStr(),"Exception");
  else setType(message.getType());
}

// CLOSE
Close::Close(const std::uint16_t& code,const std::string& reason,const bool& remote,const std::string& to) : Message(TYPE::Close,Json::Value{}, to)
{
  addKey("Code",code);
  addKey("Reason",reason);
  addKey("Remote",remote);
}

std::uint16_t Close::getCode() const
{
  return m_Value["Content"]["Code"].asInt();
}

std::string Close::getCodeStr() const
{
  return m_Value["Content"]["Code"].asString();
}

std::string Close::getReasonStr() const
{
  return m_Value["Content"]["Reason"].asString();
}

bool Close::getRemote() const
{
  return m_Value["Content"]["Remote"].asBool();
}

std::string Close::getRemoteStr() const
{
  return m_Value["Content"]["Remote"].asString();
}

Close::Close(const Message& message)
{
  setContent(message.getContentAsJson());
  setFrom(message.getFromStr());
  setTo(message.getToStr());
  if(message.getType()!=TYPE::Close) throw Exception(StatusCode::INVALID_CONVERSION,"Impossible to convert \"{}\" to \"{}\"",getTypeStr(),"Close");
  else setType(message.getType());
}

// CONNECTIONERROR
ConnectionError::ConnectionError(const std::uint32_t& retries, const double& wait_time, const int& http_status, const std::string& reason, const bool& decompressionError,const std::string& to) : Message(TYPE::ConnectionError,Json::Value{}, to)
{
  addKey("Retries",retries);
  addKey("Wait Time",wait_time);
  addKey("HTTP Status",http_status);
  addKey("Reason",reason);
  addKey("Decompression Error",decompressionError);
}

std::uint32_t ConnectionError::getRetries() const
{
  return m_Value["Content"]["Retries"].asUInt();
}

double ConnectionError::getWaitTime() const
{
  return m_Value["Content"]["Wait Time"].asDouble();
}

int ConnectionError::getHTTPStatus() const
{
  return m_Value["Content"]["HTTP Status"].asInt();
}

std::string ConnectionError::getReason() const
{
  return m_Value["Content"]["Reason"].asString();
}

bool ConnectionError::getCompressionError() const
{
  return m_Value["Content"]["Decompression Error"].asBool();
}

ConnectionError::ConnectionError(const Message& message)
{
  setContent(message.getContentAsJson());
  setFrom(message.getFromStr());
  setTo(message.getToStr());
  if(message.getType()!=TYPE::ConnectionError) throw Exception(StatusCode::INVALID_CONVERSION,"Impossible to convert \"{}\" to \"{}\"",getTypeStr(),"ConnectionError");
  else setType(message.getType());
}

// PING
Ping::Ping(const std::string& content, const std::string& to) : Message(TYPE::Ping,content, to)
{

}

Ping::Ping(const Message& message)
{
  setContent(message.getContentAsJson());
  setFrom(message.getFromStr());
  setTo(message.getToStr());
  if(message.getType()!=TYPE::Ping) throw Exception(StatusCode::INVALID_CONVERSION,"Impossible to convert \"{}\" to \"{}\"",getTypeStr(),"Ping");
  else setType(message.getType());
}

// PONG
Pong::Pong(const std::string& content, const std::string& to) : Message(TYPE::Pong,content, to)
{

}

Pong::Pong(const Message& message)
{
  setContent(message.getContentAsJson());
  setFrom(message.getFromStr());
  setTo(message.getToStr());
  if(message.getType()!=TYPE::Pong) throw Exception(StatusCode::INVALID_CONVERSION,"Impossible to convert \"{}\" to \"{}\"",getTypeStr(),"Pong");
  else setType(message.getType());
}

//State
//FIXME should have a proper ["content"]["State"] NEED TO FIX THE WEBPAGE
State::State(const STATE& state, const std::string& to): Message(TYPE::State,Json::Value{} , to)
{
  addKey("State",std::string(magic_enum::enum_name(state)));
}

STATE State::getState() const
{
  return magic_enum::enum_cast<STATE>(getStateStr()).value();
}

std::string State::getStateStr() const
{
  return m_Value["Content"]["State"].asString();
}

State::State(const Message& message)
{
  setContent(message.getContentAsJson());
  setFrom(message.getFromStr());
  setTo(message.getToStr());
  if(message.getType()!=TYPE::State) throw Exception(StatusCode::INVALID_CONVERSION,"Impossible to convert \"{}\" to \"{}\"",getTypeStr(),"State");
  else setType(message.getType());
}

Action::Action(const ACTION& action, const std::string& to): Message(TYPE::Action, Json::Value{}, to)
{
  addKey("Action",std::string(magic_enum::enum_name(action)));
}

ACTION Action::getAction() const
{
  return magic_enum::enum_cast<ACTION>(getActionStr()).value();
}

std::string Action::getActionStr() const
{
  return m_Value["Content"]["Action"].asString();
}

Action::Action(const Message& message)
{
  setContent(message.getContentAsJson());
  setFrom(message.getFromStr());
  setTo(message.getToStr());
  if(message.getType()!=TYPE::Action) throw Exception(StatusCode::INVALID_CONVERSION,"Impossible to convert \"{}\" to \"{}\"",getTypeStr(),"Action");
  else setType(message.getType());
}



Command::Command(const std::string& content, const std::string& to): Message(TYPE::Command, Json::Value(), to)
{
  if(content!="")
  {
    Json::String errs;
    bool         ok = m_Reader->parse(&content[0], &content[content.size()], &m_Value["Content"], &errs);
    if(!ok) { m_Value["Content"]=content; }
  }

  //m_Value["Content"] = content;
}

/*
std::string Command::getCommand()
{
  return m_Value["Content"].asString();
}
std::string Command::getCommand() const
{
  return getCommand();
}*/
Command::Command(const Message& message)
{
  setContent(message.getContentAsJson());
  setFrom(message.getFromStr());
  setTo(message.getToStr());
  if(message.getType()!=TYPE::Command) throw Exception(StatusCode::INVALID_CONVERSION,"Impossible to convert \"{}\" to \"{}\"",getTypeStr(),"Command");
  else setType(message.getType());
}



Data::Data(const std::string& content, const std::string& to): Message(TYPE::Data, content, to) {}
Data::Data(const Json::Value& content, const std::string& to): Message(TYPE::Data, content, to) {}

Data::Data(const Message& message)
{
  setContent(message.getContentAsJson());
  setFrom(message.getFromStr());
  setTo(message.getToStr());
  if(message.getType()!=TYPE::Data) throw Exception(StatusCode::INVALID_CONVERSION,"Impossible to convert \"{}\" to \"{}\"",getTypeStr(),"Data");
  else setType(message.getType());
}


Unknown::Unknown(const std::string& content, const std::string& to): Message(TYPE::Data, content, to) {}

Unknown::Unknown(const Message& message)
{
  setContent(message.getContentAsJson());
  setFrom(message.getFromStr());
  setTo(message.getToStr());
  if(message.getType()!=TYPE::Unknown) throw Exception(StatusCode::INVALID_CONVERSION,"Impossible to convert \"{}\" to \"{}\"",getTypeStr(),"Unknown");
  else setType(message.getType());
}


Response::Response(const std::string& content, const std::string& to): Message(TYPE::Response, content, to) {}

Response::Response(const Message& message)
{
  setContent(message.getContentAsJson());
  setFrom(message.getFromStr());
  setTo(message.getToStr());
  if(message.getType()!=TYPE::Response) throw Exception(StatusCode::INVALID_CONVERSION,"Impossible to convert \"{}\" to \"{}\"",getTypeStr(),"Response");
  else setType(message.getType());
}

}

std::ostream& operator<<(std::ostream& os, const yaodaq::Message& message)
{
  os<<message.getStyled(" ");
  return os;
}
