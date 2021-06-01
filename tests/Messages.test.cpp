#include "doctest/doctest.h"

#include <iostream>

#include "Message.hpp"

TEST_CASE("Test Message")
{
  yaodaq::Log log("Hello world");
  std::cout<<log<<std::endl;

  yaodaq::Log error(yaodaq::LEVEL::Error,"This is an error !!!");
  error.setFrom("toto/titi/tata");
  std::cout<<error<<std::endl;

}
