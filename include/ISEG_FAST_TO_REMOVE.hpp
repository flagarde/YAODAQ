#pragma once
#include "Module.hpp"
#include "WebSocketClient.hpp"
#include <ixwebsocket/IXHttpClient.h>

#include <memory>

namespace yaodaq
{

class ISEG: public Module
{
public:
  ISEG(const std::string& name = "", const std::string& type = "Default");
  void setUser(const std::string& name);
  void setPassword(const std::string& name);
  virtual ~ISEG() = default;

protected:
  void onData(const Data&) override;
  void DoConfigure() override;
  void DoStart() override;
  void DoStop() override;
  void                                        DoLoopOnStart() override;

  void                                        DoAtFirstStart() override;
private:
  std::string m_user{"admin"};
  std::string m_pass{"password"};
  std::string m_key;
  WebSocketClient m_client;
  ix::HttpClient m_http;

};

}
