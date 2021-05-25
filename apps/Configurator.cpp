#include "Configurator.hpp"

#include "CLI/CLI.hpp"
#include "ProgramInfos.hpp"

using namespace yaodaq;

int main(int argc, char** argv)
{
  ProgramInfos infos;
  infos.Logo();
  Interrupt interrupt;
  CLI::App  app{"Configurator"};
  int       port{GeneralParameters::getPort()};
  app.add_option("-p,--port", port, "Port to listen")->check(CLI::Range(0, 65535));
  std::string host{GeneralParameters::getHost()};
  app.add_option("-i,--ip", host, "IP of the server")->check(CLI::ValidIPV4);
  int backlog{ix::SocketServer::kDefaultTcpBacklog};
  app.add_option("-b,--backlog", backlog, "Backlog")->check(CLI::Range(0, 5));
  std::size_t maxConnections{ix::SocketServer::kDefaultMaxConnections};
  app.add_option("-m,--max", maxConnections, "Maximun connections")->check(CLI::PositiveNumber);
  int handshakeTimeoutSecs{3};
  app.add_option("-t,--timeout", handshakeTimeoutSecs, "Timeout in seconds")->check(CLI::PositiveNumber);
  std::string verbosity{"trace"};
  app.add_option("-v,--verbosity", verbosity, "Verbosity")->check(
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
    return app.exit(e);
  }

  GeneralParameters::setPort(port);
  GeneralParameters::setHost(host);

  Configurator configurator(port, host, backlog, maxConnections, handshakeTimeoutSecs);
  configurator.setVerbosity(verbosity);
  return configurator.loop();
}
