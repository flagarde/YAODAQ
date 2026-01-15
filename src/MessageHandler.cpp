#include "MessageHandler.hpp"

#include <thread>
#include <chrono>

#include "Signals.hpp"

#include "magic_enum/magic_enum.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "fmt/color.h"

#include "StatusCode.hpp"
#include "ixwebsocket/IXWebSocketCloseConstants.h"

#include <iostream>
#include <tuple>

namespace yaodaq
{

  std::string MessageHandler::Send(const std::string &request)
  {
    Command command(request);
    send(command);
    std::string id = command.getContentAsJson()["id"].asString();
    while(m_Lists[id].size()!=0)
    {
      //Supress the client that has send a response
      for(std::size_t i=0; i!= m_Responses[id].size();++i)
      {
        if(m_Lists[id].has(m_Responses[id][i].getFromStr())==true) m_Lists[id].erase(m_Responses[id][i].getFromStr());
      }
      std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
    nlohmann::json j;
    j["jsonrpc"] = "2.0";
    j["id"] = id;
    j["result"] = nlohmann::json::array();
    for(std::size_t i=0;i!=m_Responses[id].size();++i)
    {
      nlohmann::json object;
      object["identifier"] = m_Responses[id][i].getFromStr();
      nlohmann::json response =  nlohmann::json::parse(m_Responses[id][i].getContent());
      if(response.contains("result")) object["result"]=response["result"];
      else object["error"]=response["error"];
      j["result"].push_back(object);
    }
    m_Responses.erase(id);
    return j.dump();
  }

  bool MessageHandler::AddMethod(const std::string &name, jsonrpccxx::MethodHandle callback, const jsonrpccxx::NamedParamMapping &mapping)
  {
    return m_RPCServer.Add(name,callback,mapping);
  }

  bool MessageHandler::AddMethod(const std::string &name, jsonrpccxx::NotificationHandle callback, const jsonrpccxx::NamedParamMapping &mapping)
  {
    return m_RPCServer.Add(name,callback,mapping);
  }



  MessageHandler::MessageHandler(const Identifier& identifier) : m_LoggerHandler(identifier), m_RPCClient(*this,jsonrpccxx::version::v2)/*, m_JsonRPCClient(m_JsonFormatHandler)*/
  {
    m_Interrupt.init();
    addSink(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
  }

  /*std::vector<std::string> MessageHandler::getMethodNames(const bool includeHidden)
  {
    for(std::size_t i=0;i!=getDispatcher().GetMethodNames(includeHidden).size();i++) std::cout<<i<<" "<<getDispatcher().GetMethodNames(includeHidden)[i]<<std::endl;
    return getDispatcher().GetMethodNames(includeHidden);
  }*/

 /* std::string MessageHandler::help(const std::string& methodName)
  {
    std::vector<std::vector<jsonrpc::Value::Type>> toto = getDispatcher().GetMethod(methodName).GetSignatures();
    std::cout<<"SSSS"<<std::endl;
    for(std::size_t i=0;i!=toto.size();++i)
    {
      for(std::size_t j=0;j!=toto[i].size();++j)
      {
        std::cout<<i<<"  "<<j<<"  "<<static_cast<std::string>(magic_enum::enum_name(toto[i][j]))<<std::endl;
      }
    }
    std::cout<<"SSSSENDL"<<std::endl;
    return getDispatcher().GetMethod(methodName).GetHelpText();
  }*/


  void MessageHandler::printLog(const Log& log)
  {
    switch(log.getLevel())
    {
      case LEVEL::Trace :
      {
        if(log.getFromStr()==getIdentifier().get()) logger()->trace(fmt::format(fmt::emphasis::bold,log.getLogStr()));
        else logger()->trace("From {} : {}",fmt::format(fmt::emphasis::bold,log.getFromStr()),fmt::format(fmt::emphasis::bold,log.getLogStr()));
        break;
      }
      case LEVEL::Debug :
      {
        if(log.getFromStr()==getIdentifier().get()) logger()->debug(fmt::format(fg(fmt::color::turquoise) | fmt::emphasis::bold,log.getLogStr()));
        else logger()->debug("From {} : {}",fmt::format(fmt::emphasis::bold,log.getFromStr()),fmt::format(fg(fmt::color::turquoise) | fmt::emphasis::bold,log.getLogStr()));
        break;
      }
      case LEVEL::Info :
      {
        if(log.getFromStr()==getIdentifier().get()) logger()->info(fmt::format(fg(fmt::color::green) | fmt::emphasis::bold,log.getLogStr()));
        else logger()->info("From {} : {}",fmt::format(fmt::emphasis::bold,log.getFromStr()),fmt::format(fg(fmt::color::green) | fmt::emphasis::bold,log.getLogStr()));
        break;
      }
      case LEVEL::Warn :
      {
        if(log.getFromStr()==getIdentifier().get()) logger()->warn(fmt::format(fg(fmt::color::orange) | fmt::emphasis::bold,log.getLogStr()));
        else logger()->warn("From {} : {}",fmt::format(fmt::emphasis::bold,log.getFromStr()),fmt::format(fg(fmt::color::orange) | fmt::emphasis::bold,log.getLogStr()));
        break;
      }
      case LEVEL::Error :
      {
        if(log.getFromStr()==getIdentifier().get()) logger()->error(fmt::format(fg(fmt::color::red) | fmt::emphasis::bold,log.getLogStr()));
        else logger()->error("From {} : {}",fmt::format(fmt::emphasis::bold,log.getFromStr()),fmt::format(fg(fmt::color::red) | fmt::emphasis::bold,log.getLogStr()));
        break;
      }
      case LEVEL::Critical :
      {
        if(log.getFromStr()==getIdentifier().get()) logger()->critical(fmt::format(bg(fmt::color::red) | fmt::emphasis::bold,log.getLogStr()));
        else logger()->critical("From {} : {}",fmt::format(fmt::emphasis::bold,log.getFromStr()),fmt::format(bg(fmt::color::red) | fmt::emphasis::bold,log.getLogStr()));
        break;
      }
    }
  }

  // Command
  void MessageHandler::onCommand(const Command& command)
  {
    Response response(m_RPCServer.HandleRequest(command.getContentStr()));
    response.setTo(command.getFromStr());
    send(response);
  }

  void MessageHandler::printCommand(const Command& command)
  {
    std::string params;
    // std::cout<<command<<std::endl;
    for(Json::Value::ArrayIndex i=0;i!=command.getContentAsJson()["params"].size();++i) params+=command.getContentAsJson()["params"][i].asString()+" ";
    if(command.getFromStr()==getIdentifier().get())
    {
      logger()->info(
        fmt::format(fg(fmt::color::green) | fmt::emphasis::bold,
                    "Method : {}\nParameters : {}\nID : {}",
                    command.getContentAsJson()["method"].asString(),
                    params,
                    command.getContentAsJson()["id"].asString()
        )
      );
    }
    else logger()->info(
      "From {} : \n{}",
      fmt::format(fmt::emphasis::bold,command.getFromStr()),
                        fmt::format(fg(fmt::color::green) | fmt::emphasis::bold,"Method : {}\nParameters : {}\nID : {}",command.getContentAsJson()["method"].asString(),params,command.getContentAsJson()["id"].asString())
    );
  }


  //Response
  void MessageHandler::printResponse(const Response& response)
  {
    //std::cout<<response<<std::endl;
    /*if(response.getFromStr()==getIdentifier().get())
    {
      logger()->info("ID : {}\nResult : {}",response.getContentAsJson()["id"].asString(),response.getContentAsJson()["result"].asString());
    }
    else logger()->info("From {} : \nID : {}\nResult : {}", fmt::format(fmt::emphasis::bold,response.getFromStr()),response.getContentAsJson()["id"].asString(),response.getContentAsJson()["result"].asString());*/
  }

  void MessageHandler::onResponse(const Response& response)
  {
    printResponse(response);
    m_Responses[response.getContentAsJson()["id"].asString()].push_back(response);
  }


  void MessageHandler::onUnknown(const Unknown& unknown)
  {
    logger()->info("{}", unknown.get());
  }

  //If the module is a logger then print otherwise skip (even if the server will send nothing normally)
  void MessageHandler::onLog(const Log& log)
  {
    if(getIdentifier().getClass()==CLASS::Logger) printLog(log);
  }



  void MessageHandler::onClose(const Close& close)
  {
    if(getIdentifier().getClass()==CLASS::Logger) printClose(close);
  }


  void MessageHandler::sendException(Except& exception)
  {

  }

  void MessageHandler::onException(const Except& exception)
  {
    printException(exception);
  }


  void MessageHandler::printException(const Except& exception)
  {
    if(exception.getFromStr()==getIdentifier().get()) logger()->error(fmt::format(fg(fmt::color::red) | fmt::emphasis::bold,"Exception :\n{}",exception.getWhatStr()));
    else logger()->error(fmt::format(fg(fmt::color::red) | fmt::emphasis::bold,"{} had exception :\n{}",exception.getFromStr(),exception.getWhatStr()));
  }

  void MessageHandler::printOpen(const Open& open)
  {
    std::string headers;
    for(auto it: open.getHeaders())
    {
      headers+=fmt::format("\t{} : {}\n", it.first,it.second);
    }
    logger()->info(fmt::format(fg(fmt::color::green) | fmt::emphasis::bold,"New connection :\nID : {}\nUri : {}\nHeaders :\n{}Key : {}\n", open.getIdStr(),open.getURIStr(),headers,open.getKeyStr()));
  }

  //FIXME getFromStr() gives ///+Identifier so we cannot compare with getIdentifier !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  void MessageHandler::printClose(const Close& close)
  {
    std::string message;
    if(close.getFromStr()==getIdentifier().get()) message = fmt::format("Closed with code {}, reason {}",fmt::format(fmt::emphasis::bold,close.getCodeStr()),fmt::format(fmt::emphasis::bold,close.getReasonStr()));
    else message = fmt::format("{} closed with code {}, reason {}",fmt::format(fmt::emphasis::bold,close.getFromStr()),fmt::format(fmt::emphasis::bold,close.getCodeStr()),fmt::format(fmt::emphasis::bold,close.getReasonStr()));
    if(close.getCode() == static_cast<int16_t>(StatusCode::ALREADY_PRESENT))
    {
      logger()->error(message);
    }
    else
    {
      if(close.getCode()==ix::WebSocketCloseConstants::kNormalClosureCode)
      {
        logger()->info(message);
      }
      else
      {
        logger()->warn(message);
      }
    }
  }

  void MessageHandler::printConnectionError(const ConnectionError& connectionError)
  {
    logger()->error(fmt::format(fg(fmt::color::red) | fmt::emphasis::bold,"{}\n#retries : {}\nWait time(ms) : {}\nHTTP Status : {}\nCompressionError : {}",connectionError.getReason(),connectionError.getRetries(),connectionError.getWaitTime(),connectionError.getHTTPStatus(),connectionError.getCompressionError()));
  }

  void MessageHandler::printPing(const Ping& ping)
  {
    logger()->info(ping.getContent());
  }

  void MessageHandler::printPong(const Pong& pong)
  {
    logger()->info(pong.getContent());
  }

  void MessageHandler::onPing(const Ping& ping)
  {
    printPing(ping);
  }

  void MessageHandler::onPong(const Pong& pong)
  {
    printPong(pong);
  }

  void MessageHandler::printState(const State& state)
  {
    if(state.getFromStr()==getIdentifier().get()) logger()->warn( fmt::format("On state {}",fmt::format(fg(fmt::color::orange)|fmt::emphasis::bold,state.getStateStr() ) ));
    else logger()->warn( fmt::format("{} on state {}",fmt::format(fmt::emphasis::bold,state.getFromStr() ),fmt::format(fg(fmt::color::orange)|fmt::emphasis::bold,state.getStateStr()) ));
  }

  void MessageHandler::onState(const State& state)
  {
    printState(state);
  }

  void MessageHandler::printAction(const Action& action)
  {
    logger()->warn( fmt::format("{} sent action {}",fmt::format(fmt::emphasis::bold,action.getFromStr() ),fmt::format(fg(fmt::color::orange)|fmt::emphasis::bold,action.getActionStr()) ));
  }

  void MessageHandler::onAction(const Action& action)
  {
    printAction(action);
  }

  void MessageHandler::printData(const Data& data)
  {
    std::string size=fmt::format("{:03.2f}",data.getContentSize() / 1048576.0);
    if(data.getFromStr()==getIdentifier().get()) logger()->warn("Sending data ({})",fmt::format(fg(fmt::color::orange)|fmt::emphasis::bold,(size+ " Mo") ) );
    else logger()->warn(fmt::format(fmt::emphasis::bold,"{} Sending data ({})",fmt::format(fmt::emphasis::bold,data.getFromStr() ),fmt::format(fg(fmt::color::orange)|fmt::emphasis::bold,(size+ " Mo")) ));
  }

  void MessageHandler::onData(const Data& data)
  {
    printData(data);
  }

  void MessageHandler::onRaisingSignal()
  {
    static SIGNAL signal;
    do
    {
      signal=m_Interrupt.getSignal();
      std::this_thread::sleep_for(std::chrono::microseconds(5));
    }
    while(signal== yaodaq::SIGNAL::NO);
    int value = magic_enum::enum_integer(signal);
    if(value>=magic_enum::enum_integer(yaodaq::SEVERITY::Critical))
    {
      logger()->critical("Signal {} raised !",SignalName[signal]);
    }
    else if (value>=magic_enum::enum_integer(yaodaq::SEVERITY::Error))
    {
      logger()->error("Signal {} raised !",SignalName[signal]);
    }
    //Should be triggered by user so one character will appears -> Need to return line !
    else if (value>=magic_enum::enum_integer(yaodaq::SEVERITY::Warning))
    {
      fmt::print("\n");
      logger()->warn("Signal {} raised !",SignalName[signal]);
    }
    else if (value>=magic_enum::enum_integer(yaodaq::SEVERITY::Info))
    {
      fmt::print("\n");
      logger()->info("Signal {} raised !",SignalName[signal]);
    }
    else
    {
      fmt::print("\n");
      logger()->trace("Signal {} raised !",SignalName[signal]);
    }
    if(magic_enum::enum_integer(signal)>=magic_enum::enum_integer(SEVERITY::Critical)) std::exit(magic_enum::enum_integer(signal));
  }

}
