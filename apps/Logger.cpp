#include "Logger.hpp"

#include "CLI/CLI.hpp"
#include "Interrupt.hpp"
#include "spdlog.h"

int main(int argc, char** argv)
{
  Interrupt interrupt;
  CLI::App  app{"Logger"};
  int       port{8282};
  app.add_option("-p,--port", port, "Port to listen")->check(CLI::Range(0, 65535));
  std::string host{"127.0.0.1"};
  app.add_option("-i,--ip", host, "IP of the server")->check(CLI::ValidIPV4);
  std::string loggerName = "MyLogger";
  app.add_option("-n,--name", loggerName, "Name of the logger")
      ->check(
          [](const std::string& t) {
            if(t == "") return "Name is empty";
            else
              return "";
          },
          "Not Empty", "Test is name is empty");
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
  spdlog::info("Logger listening on IP {0} Port {1}", host, port);

  Logger logger(loggerName);
  while(true) { std::this_thread::sleep_for(std::chrono::milliseconds(10)); }

  return 0;
}
