#include "doctest/doctest.h"

#include <iostream>
#include <string>
#include <vector>

#include "GeneralParameters.hpp"

using namespace yaodaq;

TEST_CASE("Test GeneralParameters")
{
  std::cout<<GeneralParameters::getConfigsPath()<<std::endl;
  std::cout<<GeneralParameters::getPluginsPath()<<std::endl;
  std::cout<<GeneralParameters::getDataPath()<<std::endl;
  GeneralParameters::reloadParameters("./configs/.env");
  std::cout<<GeneralParameters::getConfigsPath()<<std::endl;
  std::cout<<GeneralParameters::getPluginsPath()<<std::endl;
  std::cout<<GeneralParameters::getDataPath()<<std::endl;
}
