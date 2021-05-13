#include "CLI/CLI.hpp"
#include "DAQFile.hpp"
#include "FileWritter.hpp"
#include "ProgramInfos.hpp"

using namespace yaodaq;

int main(int argc, char** argv)
{
  ProgramInfos infos;
  infos.Logo();
  CLI::App  app{"DAQFileWritter"};
  int       port{GeneralParameters::getPort()};
  app.add_option("-p,--port", port, "Port to listen")->check(CLI::Range(0, 65535));
  GeneralParameters::setPort(port);
  std::string host{GeneralParameters::getHost()};
  app.add_option("-i,--ip", host, "IP of the server")->check(CLI::ValidIPV4);
  GeneralParameters::setHost(host);
  std::string filewritterName = "FileWritter";
  app.add_option("-n,--name", filewritterName, "Name of the mode")
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

  Board::setConfigFile("../confs/Configs.toml");

  FileWritter digitizer(filewritterName);
  digitizer.setFile(std::make_unique<DAQFile>("Run${ID}.root", "RECREATE", "Run${ID}", 9));
  return digitizer.loop();
}
