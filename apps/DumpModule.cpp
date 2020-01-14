#include "Module.hpp"
#include <string>

int main()
{
  Message a;
  a.setContent("Toto");
  
  Infos b;
  b.setContent("I inform you");
  
  std::cout<<a.print()<<std::endl;
  std::cout<<b.print()<<std::endl;
  
  WebsocketClient::setURL("ws://192.168.1.126:8080/");
  Module toto("Digitizer","MyDigitizerCheriDamour");
  
  toto.sendText(a);
  toto.sendText(b);
  toto.sendBinary(a);
  toto.sendBinary(b);
}
