#pragma once

#include <functional>
#include <memory>

#include "WebSocketClient.hpp"
#include "MessageHandler.hpp"
#include "Message.hpp"

namespace ix
{
  class WebSocketMessage;
  using WebSocketMessagePtr = std::unique_ptr<WebSocketMessage>;
};

namespace yaodaq
{
  class MessageHandlerClient : public WebSocketClient, public MessageHandler
  {
  public:
    MessageHandlerClient(const Identifier&);

    virtual void onOwnOpen(const ix::WebSocketMessagePtr&);
    virtual void onOwnClose(const ix::WebSocketMessagePtr&);
    virtual void onOwnConnectionError(const ix::WebSocketMessagePtr&);
    virtual void onOwnPing(const ix::WebSocketMessagePtr&);
    virtual void onOwnPong(const ix::WebSocketMessagePtr&);
    virtual void onOwnMessage(const ix::WebSocketMessagePtr&);
    virtual void onOwnFragment(const ix::WebSocketMessagePtr&);



    // Open
    virtual void onOpen(const Open&);

    virtual void sendPing(const std::string&);
    virtual void sendState(const States&);

    virtual void onAction(const Action&);


    virtual void sendData(Data&);

    // Send commands
    void send(Message& message);
    void sendText(Message& message);
    void sendBinary(Message& message);

    //Send Log
    virtual void sendLog(Log& log) final;






  protected:
    std::function<void(const ix::WebSocketMessagePtr&)> getMessageCallback();
  private:
    std::function<void(const ix::WebSocketMessagePtr&)> m_MessageCallback;
  };
};
