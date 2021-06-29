#include "doctest/doctest.h"

#include <iostream>

#include "Message2.hpp"
#include "Classes.hpp"
#include "Infos.hpp"

using namespace yaodaq;

TEST_CASE("Test Message")
{
  Message2 message(TYPE::Log,"LL");
  std::cout<<"1"<<std::endl;
  std::cout<<message<<std::endl;
  std::cout<<"From : *"<<message.getFrom()<<"*  *"<<message.getFromStr()<<"*"<<std::endl;
  std::cout<<"To : *"<<message.getTo()<<"*  *"<<message.getToStr()<<"*"<<std::endl;
  std::cout<<"Content : *"<<message.getContent()<<"*  *"<<message.getContentStr()<<"*"<<std::endl;
  std::cout<<"Timestamp : *"<<message.getTimestamp()<<"*  *"<<message.getTimestampStr()<<"*"<<std::endl;
  Message2 message2(TYPE::Log,message.get());
  std::cout<<"2"<<std::endl;
  std::cout<<message2<<std::endl;
  std::cout<<"From : *"<<message2.getFrom()<<"*  *"<<message2.getFromStr()<<"*"<<std::endl;
  std::cout<<"To : *"<<message2.getTo()<<"*  *"<<message2.getToStr()<<"*"<<std::endl;
  std::cout<<"Content : *"<<message2.getContent()<<"*  *"<<message2.getContentStr()<<"*"<<std::endl;
  std::cout<<"Timestamp : *"<<message2.getTimestamp()<<"*  *"<<message2.getTimestampStr()<<"*"<<std::endl;

  Message2 message4(text);
  std::cout<<"4"<<std::endl;
  std::cout<<message4<<std::endl;

   Message2 message5(TYPE::Log,"");
   Infos infos1("room1","rack1","crate1",1,CLASS::Unknown,"type1","name");
   Infos infos2("room2","rack2","crate2",2,CLASS::Unknown,"type2","name2");
   message5.addTo(infos1);
   message5.addTo(infos2);
     std::cout<<"5"<<std::endl;
   std::cout<<message5<<std::endl;
  std::cout<<"From : *"<<message5.getFrom()<<"*  *"<<message5.getFromStr()<<"*"<<std::endl;
  std::cout<<"To : *"<<message5.getTo()<<"*  *"<<message5.getToStr()<<"*"<<std::endl;
  std::cout<<"Content : *"<<message5.getContent()<<"*  *"<<message5.getContentStr()<<"*"<<std::endl;
  std::cout<<"Timestamp : *"<<message5.getTimestamp()<<"*  *"<<message5.getTimestampStr()<<"*"<<std::endl;
}
