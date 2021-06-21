#pragma once

#include "ixwebsocket/IXWebSocketServer.h"

#include "MessageHandler.hpp"
#include "Clients.hpp"

#include <functional>
#include <memory>
#include <string>

namespace ix
{
  class ConnectionState;
  class WebSocket;
  class WebSocketMessage;
  using WebSocketMessagePtr = std::unique_ptr<WebSocketMessage>;
};

namespace yaodaq
{
  class MessageHandlerServer :  public MessageHandler
  {
  public:
    MessageHandlerServer(const Identifier& identifier);

    // Send command
    void send(Message&) final;
    void sendText(Message&) final;
    void sendBinary(Message&) final;




    void sendToName(Message&,const std::string&);


    //sendLog
    virtual void sendLog(Log& log) final;


    void sendToLogger(Message& message);

    void sendToLogger(const Message& message);

    // Command
    void onCommand(const Command& command) override
    {
      Response response(m_RPCServer.HandleRequest(command.getContentStr()));
      response.setTo(command.getFromStr());
      sendToName(response,command.getFromStr());
    }

    void sendClose(const Close&);

    void onLog(const Log& log);

    virtual void onOpen(std::shared_ptr<ix::ConnectionState>,ix::WebSocket&, const ix::WebSocketMessagePtr&);
    virtual void onClose(std::shared_ptr<ix::ConnectionState>,ix::WebSocket&, const ix::WebSocketMessagePtr&);
    virtual void onConnectionError(std::shared_ptr<ix::ConnectionState>,ix::WebSocket&, const ix::WebSocketMessagePtr&);
    virtual void onPing(std::shared_ptr<ix::ConnectionState>,ix::WebSocket&, const ix::WebSocketMessagePtr&);
    virtual void onPong(std::shared_ptr<ix::ConnectionState>,ix::WebSocket&, const ix::WebSocketMessagePtr&);

   /* virtual void onMessage(std::shared_ptr<ix::ConnectionState>,ix::WebSocket&, const ix::WebSocketMessagePtr&);*/

   /*


    virtual void onFragment(std::shared_ptr<ix::ConnectionState>,ix::WebSocket&, const ix::WebSocketMessagePtr&);*/
  protected:
    std::string getClient();
    /* This function fill the from key. All module sending message fill this so the only ones not filled are from browser or the webserver itself */
    void addFrom(Message&) const;
    Clients m_Clients;
    std::string m_Client;
    std::function<void(std::shared_ptr<ix::ConnectionState>,ix::WebSocket&, const ix::WebSocketMessagePtr&)> getMessageCallback();
  private:
    std::function<void(std::shared_ptr<ix::ConnectionState>,ix::WebSocket&, const ix::WebSocketMessagePtr&)> m_MessageCallback;
    int                                   m_BrowserNumber{1};
    std::map<std::string,std::string> m_NotAccepted;
  };

}
