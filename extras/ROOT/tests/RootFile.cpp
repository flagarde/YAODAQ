#include "doctest.h"

#include <iostream>
#include "RootFile.hpp"

TEST_CASE("Test RootFile")
{
  RootFile file("./Data/ID${ID}/ID${ID}.root","CREATE","tototID${ID}");
  std::cout<<"Path : "<<file.getPath()<<std::endl;
  std::cout<<"Directory : "<<file.getDirectory()<<std::endl;
  std::cout<<"Name : "<<file.getName()<<std::endl;
  std::cout<<"Extension : "<<file.getExtension()<<std::endl;
  file.open();
  //file.close();
  RootFile file2("./Data/ID174/ID${ID}121.root","CREATE","tototID${ID}");
  std::cout<<"Path : "<<file2.getPath()<<std::endl;
  std::cout<<"Directory : "<<file2.getDirectory()<<std::endl;
  std::cout<<"Name : "<<file2.getName()<<std::endl;
  std::cout<<"Extension : "<<file2.getExtension()<<std::endl;
  file2.open();
}
