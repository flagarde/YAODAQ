#include "Module.hpp"
#include <string>

int main()
{
  Message a;
  a.setContent("Toto");
  
  Info b;
  b.setContent("I inform you");
  
  Status c;
  c.setContent("TOTO");
  
  
 // std::cout<<a.print()<<std::endl;
 // std::cout<<b.print()<<std::endl;
  
  //WebsocketClient::setURL("ws://192.168.1.126:8080/");
 // Module toto("Digitizer","MyDigitizerCheriDamour");
  
 // toto.sendText(a);
 // toto.sendText(b);
 // toto.sendBinary(a);
 // toto.sendBinary(b);
  
  Message C;
  C.parse(b.get());
  std::cout<<"PRINT"<<std::endl;
  C.print();
  std::cout<<C.get()<<std::endl;
  std::cout<<C.getContent()<<std::endl;
  
  
  c.print();
  
}
