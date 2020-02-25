#include "WebsocketServer.hpp"

#include "CLI/CLI.hpp"
#include "Interrupt.hpp"
#include "spdlog.h"

int main(int argc, char** argv)
{
  Interrupt interrupt;
  CLI::App  app{"Websocket Server"};
  int       port{ix::SocketServer::kDefaultPort};
  app.add_option("-p,--port", port, "Port to listen")->check(CLI::Range(0, 65535));
  std::string host{ix::SocketServer::kDefaultHost};
  app.add_option("-i,--ip", host, "IP of the server")->check(CLI::ValidIPV4);
  int backlog{ix::SocketServer::kDefaultTcpBacklog};
  app.add_option("-b,--backlog", backlog, "Backlog")->check(CLI::Range(0, 5));
  std::size_t maxConnections{ix::SocketServer::kDefaultMaxConnections};
  app.add_option("-m,--max", maxConnections, "Maximun connections")->check(CLI::PositiveNumber);
  int handshakeTimeoutSecs{3};
  app.add_option("-t,--timeout", handshakeTimeoutSecs, "Timeout in seconds")->check(CLI::PositiveNumber);
  std::string verbosity{"warning"};
  app.add_option("-v,--verbosity", verbosity, "Verbosity")
      ->check(
          [](const std::string& t) {
            if(t != "off" && t != "trace" && t != "info" && t != "debug" && t != "warning" && t != "critical") return "Wrong verbosity level";
            else
              return "";
          },
          "Verbosity level", "Verbosity level");

  try
  {
    app.parse(argc, argv);
  }
  catch(const CLI::ParseError& e)
  {
    spdlog::error("{}", e.what());
    return e.get_exit_code();
  }
  bool            stop{false};
  char            answer{'a'};
  WebsocketServer server(port, host, backlog, maxConnections, handshakeTimeoutSecs);
  server.setVerbosity(verbosity);
  server.listen();
  server.start();
  spdlog::info("Websocket server started on IP {0} Port {1}", host, port);
  while(true) { std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
  spdlog::info("Bye !");
  return 0;
}
