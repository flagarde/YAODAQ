#include "MessageHandler.hpp"

#include <thread>
#include <chrono>

#include "Signals.hpp"

#include "magic_enum.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "fmt/color.h"

#include "StatusCode.hpp"
#include "ixwebsocket/IXWebSocketCloseConstants.h"

#include <iostream>

namespace yaodaq
{

  MessageHandler::MessageHandler(const Identifier& identifier) : m_LoggerHandler(identifier), m_JsonRPCClient(m_JsonFormatHandler)
  {
    m_Interrupt.init();
    m_JsonRPCServer.RegisterFormatHandler(m_JsonFormatHandler);
    addSink(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
  }


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
    printCommand(command);
    if(getIdentifier().getClass()!=CLASS::Logger)
    {
      outputFormattedData = m_JsonRPCServer.HandleRequest(command.getContentStr());
      Response response(outputFormattedData->GetData());
      response.setTo(command.getFromStr());
      send(response);
    }
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
    if(response.getFromStr()==getIdentifier().get())
    {
      logger()->info("ID : {}\nResult : {}",response.getContentAsJson()["id"].asString(),response.getContentAsJson()["result"].asString());
    }
    else logger()->info("From {} : \nID : {}\nResult : {}", fmt::format(fmt::emphasis::bold,response.getFromStr()),response.getContentAsJson()["id"].asString(),response.getContentAsJson()["result"].asString());
  }

  void MessageHandler::onResponse(const Response& response)
  {
    printResponse(response);
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

};
