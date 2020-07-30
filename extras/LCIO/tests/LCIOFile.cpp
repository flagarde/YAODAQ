#include "LCIOFile.hpp"

#include "doctest.h"

#include <iostream>

TEST_CASE("Test LCIOFile")
{
  LCIOFile file("./Data/LCIO/ID${ID}.lcio", "WRITE_NEW");
  std::cout << "Path : " << file.getPath() << std::endl;
  std::cout << "Directory : " << file.getDirectory() << std::endl;
  std::cout << "Name : " << file.getName() << std::endl;
  std::cout << "Extension : " << file.getExtension() << std::endl;
  file.open();
  file.close();
}
