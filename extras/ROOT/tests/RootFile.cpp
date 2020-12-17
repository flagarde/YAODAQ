#include "RootFile.hpp"

#include "doctest/doctest.h"

#include <iostream>

TEST_CASE("Test RootFile")
{
  RootFile file("./Data/ROOT/ID${ID}.root", "CREATE", "tototID${ID}");
  std::cout << "Path : " << file.getPath() << std::endl;
  std::cout << "Directory : " << file.getDirectory() << std::endl;
  std::cout << "Name : " << file.getName() << std::endl;
  std::cout << "Extension : " << file.getExtension() << std::endl;
  file.open();
  file.close();
}
