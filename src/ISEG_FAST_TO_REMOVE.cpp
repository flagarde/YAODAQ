#include "ISEG_FAST_TO_REMOVE.hpp"

#include "Classes.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <thread>

namespace yaodaq
{

ISEG::ISEG(const std::string& name, const std::string& type): Module(name, type , yaodaq::CLASS::Unknown )
{
  skipConfigFile();
}

void ISEG::setUser(const std::string& name)
{
    m_user=name;
}

void ISEG::setPassword(const std::string& pass)
{
    m_pass=pass;
}

void ISEG::onData(const Data& data)
{

}

void ISEG::DoConfigure()
{
    ix::HttpRequestArgsPtr args = m_http.createRequest();
    // Timeout options
    args->connectTimeout = 1;
    args->transferTimeout = 1;
    // Redirect options
    args->followRedirects = 0;
    args->logger = [](const std::string& msg)
    {
        std::cout << msg;
    };
    ix::HttpResponsePtr out;
    std::string url = "http://192.168.50.119/api/login/"+m_user+"/"+m_pass;
    // GET request
    out = m_http.get(url, args);
    std::cout<<"status"<<out->statusCode<<std::endl; // Can be HttpErrorCode::Ok, HttpErrorCode::UrlMalformed, etc...
    //std::cout<<out->errorCode<<std::endl;; // 200, 404, etc...
    std::cout<<out->body<<std::endl;
    if(out->statusCode !=200 || out->body == "connection error") return ;
    nlohmann::json data = nlohmann::json::parse(out->body);
    std::cout<<data["i"]<<std::endl;
    m_key=data["i"];


    std::string url2("ws://192.168.50.119:8080");
    m_client.setUrl(url2);


    // Setup a callback to be fired (in a background thread, watch out for race conditions !)
    // when a message or an event (open, close, error) is received
    m_client.setOnMessageCallback([](const ix::WebSocketMessagePtr& msg)
    {
            if (msg->type == ix::WebSocketMessageType::Message)
            {
                std::cout << nlohmann::json::parse(msg->str).dump(2) << std::endl;
            }
            else if (msg->type == ix::WebSocketMessageType::Open)
            {
                std::cout << "Connection established" << std::endl;
                std::cout << "> " << std::flush;
            }
            else if (msg->type == ix::WebSocketMessageType::Error)
            {
                // Maybe SSL is not configured properly
                std::cout << "Connection error: " << msg->errorInfo.reason << std::endl;
                std::cout << "> " << std::flush;
            }
        }
    );




    //std::cout<<out->headers<<std::endl;; // All the headers in a special case-insensitive unordered_map of (string, string)
    //std::cout<<"body:"<<out->body<<std::endl;; // All the bytes from the response as an std::string
    //std::cout<<"error:"<<out->errorMsg<<std::endl;; // Descriptive error message in case of failure
    
}



void                                        ISEG::DoLoopOnStart()
{
    static bool done{false};
    nlohmann::json data;
    if(!done)
    {
    data["i"] = m_key;
    data["t"] = "request";
    data["r"] = "websocket";
    data["c"]= nlohmann::json::array();
    data["c"][0]["c"] = "getItem";
    data["c"][0]["p"]["i"] = "Status.currentMeasure";
    data["c"][0]["p"]["v"] = "";
    data["c"][0]["p"]["u"] = "";
    data["c"][0]["p"]["p"]["l"]= "*";
    data["c"][0]["p"]["p"]["a"]= "*";
    data["c"][0]["p"]["p"]["c"]= "*";

    data["c"][1]["c"] = "getItem";
    data["c"][1]["p"]["i"] = "Status.voltageMeasure";
    data["c"][1]["p"]["v"] = "";
    data["c"][1]["p"]["u"] = "";
    data["c"][1]["p"]["p"]["l"]= "*";
    data["c"][1]["p"]["p"]["a"]= "*";
    data["c"][1]["p"]["p"]["c"]= "*";
    }
//std::cout<<data.dump(2)<<std::endl;
    m_client.send(data.dump());
    std::this_thread::sleep_for(std::chrono::microseconds(1000000));
}

void                                        ISEG::DoAtFirstStart()
{
    std::cout<<"first"<<std::endl;
    std::thread startListening(&WebSocketClient::start,&m_client);
    startListening.detach();
    std::cout<<"finish"<<std::endl;
}


void ISEG::DoStart()
{

    std::cout<<"start"<<std::endl;
    while(m_client.getReadyState()!=ix::ReadyState::Open) std::this_thread::sleep_for(std::chrono::microseconds(1000));
    LoopOnStart();
    // Now that our callback is setup, we can start our background thread and receive messages
    //std::cout<<"SSS"<<std::endl;

    //std::thread startListening(&ISEG::Do,this);
    //startListening();
    //startListening.detach();
   // std::cout<<"SSS222"<<std::endl;

  //while(1)
  //{
    //std::string toto = std::string("{\"i\":\"")+m_key+std::string("\",\"t\":\"request\",\"c\":[{\"c\":\"getItem\",\"p\":{\"p\":{\"l\":\"0\",\"a\":\"1\",\"c\":\"1\"},\"i\":\"Status.voltageMeasure\",\"v\":\"\",\"u\":\"\"}}],\"r\":\"websocket\"}");
   
    //std::this_thread::sleep_for(std::chrono::seconds(5));
  //}
  //ix::HttpRequestArgsPtr args = m_http.createRequest();


    // Timeout options
    //args->connectTimeout = 1;
    //args->transferTimeout = 1;

    // Redirect options
    //args->followRedirects = 0;

    //args->logger = [](const std::string& msg)
    //{
    //    std::cout << msg;
    //};
    //while(1)
   // {
   //     ix::HttpResponsePtr out;
   //     std::string url = "http://192.168.50.119/api/getItem/"+m_key+"/*/*/*/Status.voltageMeasure";
   //     // GET request
   //     out = m_http.get(url, args);
   //     if(out->statusCode==200)
   //     {
   //         nlohmann::json data = nlohmann::json::parse(out->body);
   //         std::cout<<data.dump(2)<<std::endl;
   //     }
    //    url = "http://192.168.50.119/api/getItem/"+m_key+"/*/*/*/Status.currentMeasure";
    //    // GET request
    //    out = m_http.get(url, args);
    //    if(out->statusCode==200)
    //    {
    //        nlohmann::json data = nlohmann::json::parse(out->body);
    //        std::cout<<data.dump(2)<<std::endl;
    //    }
        //std::cout<<"status"<<out->statusCode<<std::endl;; // Can be HttpErrorCode::Ok, HttpErrorCode::UrlMalformed, etc...
        //std::cout<<out->errorCode<<std::endl;; // 200, 404, etc...
        //std::cout<<out->headers<<std::endl;; // All the headers in a special case-insensitive unordered_map of (string, string)
        //std::cout<<"body:"<<out->body<<std::endl;; // All the bytes from the response as an std::string
        //std::cout<<"error:"<<out->errorMsg<<std::endl;; // Descriptive error message in case of failure
        //std::cout<<out->uploadSize<<std::endl;; // Byte count of uploaded data
        //std::cout<<out->downloadSize<<std::endl;; // Byte count of downloaded data

    //}
    


}

void ISEG::DoStop()
{    

    //nlohmann::json data;
    //data["i"] = m_key;
    //data["t"] = "logout";
    //data["c"] = nlohmann::json::array();
    //data["r"] = "websocket";
    //m_client.send(data.dump());
    m_client.close();
}

}
