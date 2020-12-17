#include "doctest/doctest.h"

#include <iostream>
#include "StatusCode.hpp"
#include "Exception.hpp"

TEST_CASE("Test Exceptions")
{
  Exception exception(StatusCode::NOT_FOUND, "This {0} the answer : {1}", "is", 42);

  std::cout << exception.what() << std::endl;

  Exception::setStyle();
  Exception::setFormat("********{Code}****************{File}******");
  Exception exception2(StatusCode::NOT_FOUND, "The {0} is {1}", "truth", 42);

  std::cout << exception2.what() << std::endl;
  throw exception2;
}
