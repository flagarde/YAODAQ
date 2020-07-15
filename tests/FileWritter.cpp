 #include "doctest.h"
 #include "FileWritter.hpp"
 #include <iostream>
 #include "Interrupt.hpp"
 
 TEST_CASE("Test FileWritter")
 {
   Interrupt interrupt;
   class Toto : public File
   {
   public:
     Toto(const std::string& name="",const std::string& option=""):File(name,option){};
     virtual bool isOpened() final { return true;}
     virtual void open() final {std::cout<<"Open"<<std::endl;}
     virtual void close() final {std::cout<<"Close"<<std::endl;}
     virtual void parseData(const Data& data) final 
     {
       static int nevent{0};
       if(nevent==m_StopAfter) std::exit(0);
       else
       {
         std::cout<<data.get()<<std::endl;
         nevent++;
       }
     }
     virtual void setID() final {m_ID="Toto";}
   private:
     int m_StopAfter{10};
   };
   GeneralParameters::setURL("ws://202.120.13.50:9999/");
   
   Board::setConfigFile("../confs/Configs.toml");
   
   Toto toto("toto","H");
   FileWritter writer("Writer");
   writer.lookForConfiguration(false);
   writer.setFile(toto);
   interrupt.wait();
 }
