#include "Controller.hpp"

#include "CLI/CLI.hpp"
#include "ProgramInfos.hpp"
#include "magic_enum.hpp"

#include "jsonrpc/request.h"

#include <iostream>

using namespace yaodaq;

int main(int argc, char** argv)
{
  ProgramInfos infos;
  infos.Logo();
  CLI::App  app{"Controller"};
  int port{GeneralParameters::getPort()};
  app.add_option("-p,--port", port, "Port to listen")->check(CLI::Range(0, 65535));
  std::string host{GeneralParameters::getHost()};
  app.add_option("-i,--ip", host, "IP of the server")->check(CLI::ValidIPV4);
  std::string name {"Controller"};
  app.add_option("-n,--name", name, "Name of the Controller");
  std::string action;
  app.add_option("-a,--action", action, "Action");
  std::string commandName;
  app.add_option("-c,--command", commandName, "Command");
  std::vector<std::string> params;
  app.add_option("--params", params, "Parameters");
  std::string verbosity{"trace"};
  app.add_option("-v,--verbosity", verbosity, "Verbosity")->check(
    [](const std::string& t) {
      if(t != "off" && t != "trace" && t != "info" && t != "debug" && t != "warning" && t != "critical") return "Wrong verbosity level";
                                                                  else
                                                                    return "";
    },
    "Verbosity level", "Verbosity level");

  std::string log;
  app.add_option("-l,--log", log, "log to send.");
  std::string level{"Info"};
  app.add_option("--level", level, "level of the log.");

  try
  {
    app.parse(argc, argv);
  }
  catch(const CLI::ParseError& e)
  {
    spdlog::error("{}", e.what());
    return e.get_exit_code();
  }

  jsonrpc::Request::Parameters parameters;
  for(std::size_t i=0;i!=params.size();++i)
  {
    std::size_t pos = params[i].find(":");
    std::string type=params[i].substr(0,pos);
    std::string value=params[i].substr(pos+1);
    if(type=="int") parameters.push_back(static_cast<int>(std::stoi(value)));
    if(type=="double") parameters.push_back(static_cast<double>(std::stod(value)));
    if(type=="float") parameters.push_back(static_cast<float>(std::stof(value)));
    if(type=="string") parameters.push_back(value);
  }


  GeneralParameters::setPort(port);
  GeneralParameters::setHost(host);

  Controller controller(name);
  controller.setVerbosity(verbosity);

  controller.startListening();

  if(!action.empty())
  {
    controller.sendAction(action);
  }
  if(!commandName.empty())
  {
    Command command = controller.command(commandName,parameters);
    controller.send(command);
  }
  if(!log.empty())
  {
    auto _level = magic_enum::enum_cast<yaodaq::LEVEL>(level);
    if(_level.has_value())
    {
      controller.log(_level.value(),log);
    }
    else
    {
      controller.logger()->warn("{} level is unknown ! Falling back to Info",level);
      controller.log(LEVEL::Info,log);
    }
  }
  std::this_thread::sleep_for(std::chrono::microseconds(1000000));
  controller.stopListening();
}
