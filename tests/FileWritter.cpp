#include "FileWritter.hpp"

#include "Interrupt.hpp"
#include "doctest/doctest.h"

#include <iostream>

TEST_CASE("Test FileWritter")
{
  Interrupt interrupt;
  class Toto: public File
  {
  public:
    Toto(const std::string& name = "", const std::string& option = ""): File(name, option){};
    bool isOpened() final { return true; }
    void open() final { std::cout << "Open" << std::endl; }
    void close() final { std::cout << "Close" << std::endl; }
    void parseData(const Data& data) final
    {
      static int nevent{0};
      if(nevent == m_StopAfter) std::exit(0);
      else
      {
        std::cout << data.get() << std::endl;
        nevent++;
      }
    }
    void setID() final { m_ID = "Toto"; }

  private:
    int m_StopAfter{10};
  };
  GeneralParameters::setURL("ws://202.120.13.50:9999/");

  Board::setConfigFile("../confs/Configs.toml");

  Toto        toto("toto", "H");
  FileWritter writer("Writer");
  writer.setFile(std::make_unique<Toto>("toto", "H"));
  interrupt.wait();
}
