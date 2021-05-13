#include "CAENDigitizerBoard.hpp"
#include "CLI/CLI.hpp"
#include "ProgramInfos.hpp"

using namespace yaodaq;

int main(int argc, char** argv)
{
  ProgramInfos infos;
  infos.Logo();
  CLI::App  app{"Dump"};
  int       port{yaodaq::GeneralParameters::getPort()};
  app.add_option("-p,--port", port, "Port to listen")->check(CLI::Range(0, 65535));
  std::string host{yaodaq::GeneralParameters::getHost()};
  app.add_option("-i,--ip", host, "IP of the server")->check(CLI::ValidIPV4);
  std::string name = "Board1";
  app.add_option("-n,--name", name, "Name of the mode")
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

  GeneralParameters::setPort(port);
  GeneralParameters::setHost(host);

  Board::setConfigFile("../confs/Configs.toml");

  CAEN::CAENDigitizerBoard digitizer(name);

  return digitizer.loop();
}
