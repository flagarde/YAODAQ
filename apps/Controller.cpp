#include "Controller.hpp"

#include "CLI/CLI.hpp"
#include "ProgramInfos.hpp"
#include "magic_enum.hpp"

using namespace yaodaq;

int main(int argc, char** argv)
{
  ProgramInfos infos;
  infos.Logo();
  CLI::App  app{"Controller"};
  int       port{GeneralParameters::getPort()};
  app.add_option("-p,--port", port, "Port to listen")->check(CLI::Range(0, 65535));
  std::string host{GeneralParameters::getHost()};
  app.add_option("-i,--ip", host, "IP of the server")->check(CLI::ValidIPV4);
  std::string name = "Controller";
  app.add_option("-n,--name", name, "Name of the Controller")
      ->check(
          [](const std::string& t) {
            if(t == "") return "Name is empty";
            else
              return "";
          },
          "Not Empty", "Test is name is empty");
  std::string action = "";
  app.add_option("-a,--action", action, "Action");
  std::string command = "";
  app.add_option("-c,--command", command, "Command");
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

  GeneralParameters::setPort(port);
  GeneralParameters::setHost(host);

  Controller controller(name);
  controller.setVerbosity(verbosity);

  controller.startListening();
  if(action != "") controller.sendAction(action);
  if(command != "") controller.sendCommand(command);
  if(!log.empty())
  {
    auto _level = magic_enum::enum_cast<yaodaq::LEVEL>(level);
    if(_level.has_value())
    {
      controller.log(_level.value(),log);
      // color.value() -> Color::GREEN
    }
    else
    {
      controller.logger()->warn("{} level is unknown ! Falling back to Info",level);
      controller.log(LEVEL::Info,log);
    }
  }
  controller.stopListening();
  return 0;
}
