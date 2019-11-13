#include "WebServer.hpp"
#include "IXWebSocketServer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "json/json.h"


 void WebServer::SendStatus(const std::string& name)
 {
    if(status==name) return;
    if(name=="STARTED") isStarted=true;
     else if(name=="STOPED") isStarted=false;
    
    Json::StreamWriterBuilder builder;
    builder.settings_["indentation"] = "";
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    Json::Value val{};
    val["Type"] ="Status";
    val["Status"] = name;
    std::stringstream s{}; 
    writer->write(val, &s);
    for (auto&& client : m_server.getClients())
    {
        client->send(s.str());
    }
   
    resetCommand(); 
    status=name;
    std::cout<<"Status : "<<status<<" Command : "<<command<<"Is strated " <<isStarted<<std::endl;
 }
 
 void  WebServer::SendInfos(const std::string& name,const std::string& info)
 {
    Json::StreamWriterBuilder builder;
    builder.settings_["indentation"] = "";
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    Json::Value val;
    val["Type"] = "Infos";
    val["Name"] = name;
    val[name]=info;
    std::stringstream s; 
    writer->write(val, &s);
    for (auto&& client : m_server.getClients())
    {
        client->send(s.str());
    }
    resetCommand();
 }


WebServer::WebServer(int port,const std::string& host,int backlog,size_t maxConnections):m_server(port,host,backlog,maxConnections)
{
    m_server.setOnConnectionCallback([this](std::shared_ptr<ix::WebSocket> webSocket,std::shared_ptr<ix::ConnectionState> connectionState)
    {
        webSocket->setOnMessageCallback([this,webSocket, connectionState](const ix::WebSocketMessagePtr msg)
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
                Json::StreamWriterBuilder builder;
                builder.settings_["indentation"] = "";
                std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
                Json::Value val;
                val["Type"] = "Status";
                val["Status"] = status;
                val["Message"]="WebSocket Opened";
                std::stringstream s; 
                writer->write(val, &s);
                webSocket->send(s.str());
            }
            else if (msg->type == ix::WebSocketMessageType::Close)
            {

                std::cout<<"Closed"<<std::endl;
            }
            else if (msg->type == ix::WebSocketMessageType::Message)
            {
                std::cout<<"Message "<<msg->str<<std::endl;
               Json::CharReaderBuilder builder;
               builder["collectComments"] = false;
               Json::Value value;
               Json::String errs;
               std::stringstream s(msg->str); 
               bool ok = Json::parseFromStream(builder,s, &value, &errs);
               if(ok)
               {
                if(value["Type"].asString()=="Command")
                {
                    
                    command=value["Command"].asString();
                }
                if(value["Type"].asString()=="Infos") Infos=value ;
                                        std::cout<<value<<std::endl;
               }
               else
               {
                   Json::StreamWriterBuilder builder;
                    builder.settings_["indentation"] = "";
                    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
                    Json::Value val;
                    val["Type"] = "Status";
                    val["Status"] ="ALERT" ;
                    val["Message"]="<h1> Your command is not readeable !! Do you speak JSON !! I don't understand Klingon yet !!! </h1>";
                    std::stringstream s; 
                    writer->write(val, &s);
                    webSocket->send(s.str());
               }
                 /*std::cout<<":::::::"<<std::string(msg->str)<<":::::::"<<std::endl;
                    Json::CharReaderBuilder builder;
                    Json::Value obj;   // will contain the root value after parsing.
                    std::string errs;
                    std::ifstream message(std::string(msg->str),std::ifstream::binary);
                    bool ok = Json::parseFromStream(builder,message,&obj,&errs);
                    if ( !ok )
                    {
                        std::cout  << errs << "\n";
                    }
                    std::cout<<obj.asString()<<std::endl;*/
                
                
                
               /* Json::CharReaderBuilder builder;
                Json::Value obj;   // will contain the root value after parsing.
                std::string errs;
                std::ifstream message(msg->str,std::ifstream::binary);
                bool ok = Json::parseFromStream(builder,message,&obj,&errs);
                if ( !ok )
                {
                    std::cout  << errs << "\n";
                }
                std::cout<<obj.asString()<<std::endl;
                
                
                static std::string where="Void";
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
     		    }*/
            }
        }
        );
    }
);
   
}
