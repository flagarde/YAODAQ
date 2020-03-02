#include "Controller.hpp"

#include "CLI/CLI.hpp"
#include "Interrupt.hpp"

int main(int argc, char** argv)
{
  CLI::App app{"Dump"};
  int      port{8080};
  app.add_option("-p,--port", port, "Port to listen")->check(CLI::Range(0, 65535));
  std::string host{"127.0.0.1"};
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
  try
  {
    app.parse(argc, argv);
  }
  catch(const CLI::ParseError& e)
  {
    spdlog::error("{}", e.what());
    return e.get_exit_code();
  }

  WebsocketClient::setURL("ws://" + host + ":" + std::to_string(port) + "/");

  Controller toto(controllerName);
  // Interrupt  interrupt(toto);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  toto.sendAction(action);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  toto.stop();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  return 0;
}
