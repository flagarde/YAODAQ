#include "doctest/doctest.h"

#include <iostream>
#include <string>
#include <vector>
#include "Utilities.hpp"

TEST_CASE("Test Utilities")
{
  std::string string1{"0,1,2,3,4,5"};
  std::string string2{"0,1,2,three,4,5"};
  std::string one{"1"};
  std::string onePoint2{"1.2"};
  std::cout<<getFullExePath()<<std::endl;
  std::cout<<getExePath()<<std::endl;
}

