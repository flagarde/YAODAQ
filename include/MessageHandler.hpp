#pragma once

#include "LoggerHandler.hpp"
#include "Interrupt.hpp"
#include "Message.hpp"
//#include "jsonrpc/server.h"
//#include "jsonrpc/client.h"

#include <map>
#include <jsonrpccxx/client.hpp>
#include <jsonrpccxx/server.hpp>
#include "List.hpp"
#include <thread>
#include <future>

namespace yaodaq
{

  class MessageHandler : public jsonrpccxx::IClientConnector
  {
  public:




    MessageHandler(const Identifier&);

    bool AddMethod(const std::string &name, jsonrpccxx::MethodHandle callback, const jsonrpccxx::NamedParamMapping &mapping = jsonrpccxx::NAMED_PARAM_MAPPING);
    bool AddMethod(const std::string &name, jsonrpccxx::NotificationHandle callback, const jsonrpccxx::NamedParamMapping &mapping = jsonrpccxx::NAMED_PARAM_MAPPING);

    template<typename T> std::pair<std::map<Identifier,T>,std::map<Identifier,nlohmann::json>> CallMethod(const jsonrpccxx::id_type &id, const std::string &name)
    {
       m_Lists[id] = m_List;

      //Save the list of client + server that will answer;
      std::map<Identifier,T> results;
      std::map<Identifier,nlohmann::json> errors;

      nlohmann::json result = m_RPCClient.CallMethod<nlohmann::json>(id,name);
      for(std::size_t i=0; i!=result.size(); ++i)
      {
        //It can be an error or a result;
        if(result[i].contains("result")) results[Identifier(result[i]["identifier"].get<std::string>())] = result[i]["result"].get<T>();
        else errors[Identifier(result[i]["identifier"].get<std::string>())] = result[i]["error"].get<nlohmann::json>();
      }
      return std::move(std::pair<std::map<Identifier,T>,std::map<Identifier,nlohmann::json>>(results,errors));
    }
    template<typename T> std::pair<std::map<Identifier,T>,std::map<Identifier,nlohmann::json>> CallMethod(const jsonrpccxx::id_type &id, const std::string &name, const jsonrpccxx::positional_parameter &params)
    {


      m_Lists[id] = m_List;

      std::map<Identifier,T> results;
      std::map<Identifier,nlohmann::json> errors;
      nlohmann::json result = m_RPCClient.CallMethod<nlohmann::json>(id,name,params);
      for(std::size_t i=0; i!=result.size(); ++i)
      {
        //It can be an error or a result;
        if(result[i].contains("result")) results[Identifier(result[i]["identifier"].get<std::string>())] = result[i]["result"].get<T>();
        else errors[Identifier(result[i]["identifier"].get<std::string>())] = result[i]["error"].get<nlohmann::json>();
      }
      return std::move(std::pair<std::map<Identifier,T>,std::map<Identifier,nlohmann::json>>(results,errors));
    }
    template<typename T> std::pair<std::map<Identifier,T>,std::map<Identifier,nlohmann::json>> CallMethodNamed(const jsonrpccxx::id_type &id, const std::string &name, const jsonrpccxx::named_parameter &params = {})
    {

      m_Lists[id] = m_List;

      std::map<Identifier,T> results;
      std::map<Identifier,nlohmann::json> errors;
      nlohmann::json result = m_RPCClient.CallMethodNamed<nlohmann::json>(id,name,params);
      for(std::size_t i=0; i!=result.size(); ++i)
      {
        //It can be an error or a result;
        if(result[i].contains("result")) results[Identifier(result[i]["identifier"].get<std::string>())] = result[i]["result"].get<T>();
        else errors[Identifier(result[i]["identifier"].get<std::string>())] = result[i]["error"].get<nlohmann::json>();
      }
      return std::move(std::pair<std::map<Identifier,T>,std::map<Identifier,nlohmann::json>>(results,errors));
    }

    void CallNotification(const std::string &name, const jsonrpccxx::positional_parameter &params = {}) { m_RPCClient.CallNotification(name,params); }
    void CallNotificationNamed(const std::string &name, const jsonrpccxx::named_parameter &params = {}) { m_RPCClient.CallNotificationNamed(name,params); }



    // Send command
    virtual void send(Message&) = 0;
    virtual void sendText(Message&) = 0;
    virtual void sendBinary(Message&) = 0;

    // getMethodNames
    //std::vector<std::string> getMethodNames(const bool includeHidden = false);

    // getHelp
   // std::string help(const std::string& methodName);

    //******************* LOGS ******************************//
    // Log messages
    template<typename... Args> inline void log(const LEVEL& level, fmt::string_view str, const Args&... args)
    {
      Log log(level,fmt::format(str,args...));
      log.setFrom(getIdentifier().get());
      sendLog(log);
    }
    template<typename... Args> inline void log(const std::string& level, fmt::string_view str, const Args&... args)
    {
      Log log(level,fmt::format(str,args...));
      log.setFrom(getIdentifier().get());
      sendLog(log);
    }
    template<typename... Args> inline void log(fmt::string_view fmt, const Args&... args)
    {
      log(LEVEL::Info,fmt,args...);
    }

    // LEVELS
    template<typename... Args> inline void trace(fmt::string_view fmt, const Args&... args)
    {
      log(LEVEL::Trace,fmt,args...);
    }
    template<typename... Args> inline void debug(fmt::string_view fmt, const Args&... args)
    {
      log(LEVEL::Debug,fmt,args...);
    }
    template<typename... Args> inline void info(fmt::string_view fmt, const Args&... args)
    {
      log(LEVEL::Info,fmt,args...);
    }
    template<typename... Args> inline void warn(fmt::string_view fmt, const Args&... args)
    {
      log(LEVEL::Warn,fmt,args...);
    }
    template<typename... Args> inline void error(fmt::string_view fmt, const Args&... args)
    {
      log(LEVEL::Error,fmt,args...);
    }
    template<typename... Args> inline void critical(fmt::string_view fmt, const Args&... args)
    {
      log(LEVEL::Critical,fmt,args...);
    }


   /* Command command(const std::string& methodName, const jsonrpc::Request::Parameters& params = {})
    {
      std::shared_ptr<jsonrpc::FormattedData> jsonRequest = m_JsonRPCClient.BuildRequestData(methodName,params);
      Command command(jsonRequest->GetData());
      return std::move(command);
    }

    template<typename FirstType, typename... RestTypes>  Command command(const std::string& methodName, FirstType&& first, RestTypes&&... rest)
    {
      std::shared_ptr<jsonrpc::FormattedData> jsonRequest = m_JsonRPCClient.BuildRequestData(methodName,first,rest...);
      Command command(jsonRequest->GetData());
      return std::move(command);
    }
*/
    //******************************************************//
    virtual void onUnknown(const Unknown&);


    // Command
    virtual void printCommand(const Command&);
    virtual void onCommand(const Command&);


    //Response
    virtual void printResponse(const Response&);

    virtual void onResponse(const Response&);



    //*********************** Except ***********************//
    virtual void printException(const Exception& exception)
    {
      logger()->error(fmt::format(fg(fmt::color::red) | fmt::emphasis::bold,"Exception :\n{}",exception.what()));
    }
    virtual void printException(const Except&);
    //******************************************************//


    virtual void printConnectionError(const ConnectionError&);

    virtual void printPing(const Ping&);
    virtual void printPong(const Pong&);


    virtual void printState(const State&);
    virtual void onState(const State&);

    virtual void printAction(const Action&);
    virtual void onAction(const Action&);

    virtual void printData(const Data&);
    virtual void onData(const Data&);


    virtual void onPing(const Ping&);
    virtual void onPong(const Pong&);


    virtual void onClose(const Close&);


    //Dumb sender (Only print the log)
    virtual void sendLog(Log& log)
    {
      printLog(log);
    }

    virtual void sendException(Except&);

    virtual void onException(const Except&);

    virtual void printOpen(const Open&);
    virtual void printClose(const Close&);



    void setVerbosity(const std::string& verbosity)
    {
      m_LoggerHandler.setVerbosity(verbosity);
    }
    std::shared_ptr<spdlog::logger> logger()
    {
      return m_LoggerHandler.logger();
    }


    void clearSinks()
    {
      m_LoggerHandler.clearSinks();
    }

    void addSink(const spdlog::sink_ptr& sink)
    {
      m_LoggerHandler.addSink(sink);
    }


    Identifier getIdentifier() const
    {
      return m_LoggerHandler.getIdentifier();
    }


    //Action to do when a signal as been raised on the client/server
    void onRaisingSignal();


  protected:

    mutable List m_List;
    mutable std::map<jsonrpccxx::id_type,List> m_Lists;
    void printLog(const Log& log);

    //When it receive a Log
    virtual void onLog(const Log& log);

    LoggerHandler m_LoggerHandler;
    Interrupt  m_Interrupt;

  private:
    jsonrpccxx::JsonRpc2Server m_RPCServer;
    jsonrpccxx::JsonRpcClient m_RPCClient;
    std::string Send(const std::string &);
    std::map<std::string,std::vector<Response>> m_Responses;
  };
};
