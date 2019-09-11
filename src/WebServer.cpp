#include "WebServer.hpp"
#include "IXWebSocketServer.h"
#include <iostream>

 WebServer::WebServer(int port,
                     const std::string& host,
                     int backlog,
                     size_t maxConnections):m_server(port,host,backlog,maxConnections)
  {

        m_server.setOnConnectionCallback(
  [this](std::shared_ptr<ix::WebSocket> webSocket,std::shared_ptr<ix::ConnectionState> connectionState)
  {
  	webSocket->setOnMessageCallback(
        [this,webSocket, connectionState](const ix::WebSocketMessagePtr msg)
        {
        	if (msg->type == ix::WebSocketMessageType::Open)
                {
                    std::cerr << "New connection" << std::endl;

                    // A connection state object is available, and has a default id
                    // You can subclass ConnectionState and pass an alternate factory
                    // to override it. It is useful if you want to store custom
                    // attributes per connection (authenticated bool flag, attributes, etc...)
                    std::cerr << "id: " << connectionState->getId() << std::endl;

                    // The uri the client did connect to.
                    std::cerr << "Uri: " << msg->openInfo.uri << std::endl;

                    std::cerr << "Headers:" << std::endl;
                    for (auto it : msg->openInfo.headers)
                    {
                        std::cerr << it.first << ": " << it.second << std::endl;
                    }
                    webSocket->send(this->command.c_str());
                }
                else if (msg->type == ix::WebSocketMessageType::Close)
                {
			webSocket->send("NONE");
		}
                else if (msg->type == ix::WebSocketMessageType::Message)
                {
			static std::string where="Void";
                    
                    // For an echo server, we just send back to the client whatever was received by the server
                    // All connected clients are available in an std::set. See the broadcast cpp example.
                    // Second parameter tells whether we are sending the message in binary or text mode.
                    // Here we send it in the same mode as it was received.

   		    if (msg->str == "Where") 
		    {

			std::cout<<"I TRIGGER" <<msg->str<<"   "<<where<<"  "<<command <<std::endl;
                       webSocket->send(where.c_str());
    		    } 
		    else 
		    {
    			std::cout<<msg->str<<"   "<<where<<"  "<<command <<std::endl;
     	              command = msg->str;
                      where = msg->str;
      			webSocket->send(where.c_str());
     		    }




                }
            }
        );
    }
);

  }
