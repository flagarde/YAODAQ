 
 #include "IXWebSocketServer.h"
 #include <iostream>
int main(){
 
 // Run a server on localhost at a given port.
 // Bound host name, max connections and listen backlog can also be passed in as parameters.
 ix::WebSocketServer server(8080,"192.168.1.126");
 
 server.setOnConnectionCallback(
   [&server](std::shared_ptr<ix::WebSocket> webSocket,
             std::shared_ptr<ix::ConnectionState> connectionState)
   {
     webSocket->setOnMessageCallback(
       [webSocket, connectionState, &server](const ix::WebSocketMessagePtr msg)
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
         }
         else if (msg->type == ix::WebSocketMessageType::Message)
         {
           // For an echo server, we just send back to the client whatever was received by the server
           // All connected clients are available in an std::set. See the broadcast cpp example.
           // Second parameter tells whether we are sending the message in binary or text mode.
           // Here we send it in the same mode as it was received.
           webSocket->send(msg->str, msg->binary);
         }
       }
     );
   }
 );
 
 auto res = server.listen();
 if (!res.first)
 {
   std::cout<<res.second<<std::endl;
   // Error handling
   return 1;
 }
 
 // Run the server in the background. Server can be stoped by calling server.stop()
 server.start();
 
 // Block until server.stop() is called.
 server.wait();
}
