#include "Controller.hpp"

#include "CLI/CLI.hpp"
#include "ProgramInfos.hpp"

using namespace yaodaq;

int main(int argc, char** argv)
{
  ProgramInfos infos;
  infos.Logo();
  CLI::App  app{"Dump"};
  int       port{GeneralParameters::getPort()};
  app.add_option("-p,--port", port, "Port to listen")->check(CLI::Range(0, 65535));
  std::string host{GeneralParameters::getHost()};
  app.add_option("-i,--ip", host, "IP of the server")->check(CLI::ValidIPV4);
  std::string controllerName = "Controller";
  app.add_option("-n,--name", controllerName, "Name of the Controller")
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

  Controller controller(controllerName);
  controller.startListening();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  if(action != "") controller.sendAction(action);
  if(command != "") controller.sendCommand(command);
  return 0;
}
