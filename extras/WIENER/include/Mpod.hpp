#pragma once
#include "Board.hpp"

namespace WIENER
{

class Mpod : public yaodaq::Board
{
public:
  Mpod(const std::string& name = "Mpod");
  void turnOnChannel();
  void turnOffChannel();

  void DoInitialize()
  {
    addMethod();
  }

  virtual void addMethod()
  {
    std::cout<<"LLLLLLLLLLLLLLLLLLLLLLLLLLLLL"<<m_RPCServer.Add("turn", GetHandle(&Mpod::turnOnChannel,*this),{})<<std::endl;
   // std::cout<<"LLLLLLLLLLLLLLLLLLLLLLLLLLLLL"<<m_RPCServer.Add("turn", GetHandle(&Mpod::turnOffChannel,*this),{})<<std::endl;

  }


};

}
