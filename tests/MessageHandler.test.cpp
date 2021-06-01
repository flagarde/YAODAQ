//#include "doctest/doctest.h"

#include <iostream>

#include "MessageHandler.hpp"

int main(){
/*
TEST_CASE("Test Message Handler")
{*/
  yaodaq::MessageHandler messageHandler(yaodaq::Identifier(yaodaq::CLASS::Module,"MyTestModule","Toto"));
  messageHandler.log(yaodaq::LEVEL::Trace,"I'm a {}","Trace");
  messageHandler.log(yaodaq::LEVEL::Info,"I'm a {}","Info");
  messageHandler.log(yaodaq::LEVEL::Debug,"I'm a {}","Debug");
  messageHandler.log(yaodaq::LEVEL::Warn,"I'm a {}","Warn");
  messageHandler.log(yaodaq::LEVEL::Error,"I'm a {}","Error");
  messageHandler.log(yaodaq::LEVEL::Critical,"I'm a {}","Critical");
//}
}
