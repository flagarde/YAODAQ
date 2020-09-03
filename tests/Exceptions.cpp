#include "doctest.h"

#include <iostream>
#include "StatusCode.hpp"
#include "Exception.hpp"

TEST_CASE("Test Exceptions")
{
  Exception toto(StatusCode::NOT_FOUND,"Toto l'asticot {0}, {1},",21,"hhh");

  std::cout<<toto.what()<<std::endl;


  Exception::setStyle();
  Exception::setFormat("********{Code}****************{File}******");
  Exception toto2(StatusCode::NOT_FOUND,"Toto l'asticot {0}, {1},",21,"hhh");


  std::cout<<toto2.what()<<std::endl;


  throw toto;

}
