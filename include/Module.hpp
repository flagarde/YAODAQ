#pragma once

#include "MessageHandlerClient.hpp"

#include "GeneralParameters.hpp"

#include "ConfigurationLoader.hpp"
#include "Message.hpp"
#include "StatusCode.hpp"

#include <string>
#include <thread>

namespace yaodaq
{

class Module : public MessageHandlerClient
{
public:
  Module(const std::string& name = "Unknown", const std::string& type = "Module", const CLASS& _class = CLASS::Module);
  virtual ~Module() = default;


  int loop();

  void skipConfigFile();
  void                                   Initialize();
  void                                   Connect();
  void                                   Configure();
  void                                   Start();
  void                                   Pause();
  void                                   Stop();
  void                                   Clear();
  void                                   Disconnect();
  void                                   Release();
  void                                   Quit();

  void                                   LoopOnStart();
  void                                   LoopOnPause();
  STATE                                  getState();
  std::string                            getStateStr();
  static void                            setConfigFile(const std::string&);
  void                                   printParameters();
  void                                   stopListening();
  void                                   startListening();
/*
  void sendData(const std::string& dat) { Data data(dat); printData(dat); sendBinary(data); }
  void sendData(const Json::Value& dat) { Data data(dat); printData(dat); sendBinary(data); }*/

  std::string getName()
  {
    return getIdentifier().getName();
  }

  void wait();

protected:
  static ConfigurationLoader            m_Config;
  //virtual void                    onOwnMessage(const ix::WebSocketMessagePtr&) final;
  void                            LoadConfig();
  virtual void                    verifyParameters();
  toml::value                     m_Conf{""};
private:
  bool            m_UseConfigFile{true};
  void            DoDoLoopOnStart();
  void            DoDoLoopOnPause();
  STATE m_State{STATE::Uninitialized};
  std::thread     m_LoopOnStart;
  std::thread     m_LoopOnPause;
  bool            m_LoopOnStartUsed{false};
  bool            m_LoopOnPauseUsed{false};
  Module() = delete;
  virtual void                                        DoInitialize();
  virtual void                                        CallBoardConnect(){};
  virtual void                                        DoConfigure();
  virtual void                                        DoStart();
  virtual void                                        DoPause();
  virtual void                                        DoStop();
  virtual void                                        DoClear();
  virtual void                                        CallBoardDisconnect(){};
  virtual void                                        DoRelease();
  virtual void                                        DoQuit();
  virtual void                                        DoLoopOnStart();
  virtual void                                        DoLoopOnPause();
  virtual void                                        DoAtFirstStart();
  bool                                                m_IsFirstStart{true};
  void                                                onAction(const Action&) override;
  //void                                                DoOnCommand(const Message&);
  //virtual void                                        DoOnData(const Data& data);
  void                                                sendState();
  void                                                setState(const STATE&);
  //void                                                DoOnMessage(const ix::WebSocketMessagePtr&);
};

}
