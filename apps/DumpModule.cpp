#include "Module.hpp"
#include <string>

int main()
{  
 
  Status c;
  c.setContent("INITIALIZE");
  
 // std::cout<<a.print()<<std::endl;
 // std::cout<<b.print()<<std::endl;
  
  WebsocketClient::setURL("ws://192.168.1.126:8080/");
  Module Master("MaSter","MyMaster");
  Module Receiver("Receiver","MyReceiver");
 // toto.sendText(a);
 // toto.sendText(b);
 // toto.sendBinary(a);
 // toto.sendBinary(b);
  Master.sendText(c);
  
}
