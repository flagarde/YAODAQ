#include "Module.hpp"
#include <string>
#include "CAENDigitizerBoard.hpp"

int main()
{  
 
  //Status c;
  //c.setContent("INITIALIZE");
  
 // std::cout<<a.print()<<std::endl;
 // std::cout<<b.print()<<std::endl;
  
  WebsocketClient::setURL("ws://127.0.0.1:80/");
  Board::setConfigFile("../confs/Configs.toml");
 // Module Master("MaSter","MyMaster");
 // Module Receiver("Receiver","MyReceiver");
 // toto.sendText(a);
 // toto.sendText(b);
 // toto.sendBinary(a);
 // toto.sendBinary(b);
 // Master.sendText(c);
  
  
  CAEN::CAENDigitizerBoard toto("toto");
  toto.Initialize();
  //std::cout<<"LL"<<toto.SWRelease()<<std::endl;
  //std::cout<<"KK"<<toto.BoardFWRelease()<<std::endl;
  
  
  
}
