#include "CAENDigitizerBoard.hpp"
#include "CLI/CLI.hpp"
#include "ProgramInfos.hpp"
#include "TApplication.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TH1F.h"
#include "TRootCanvas.h"
#include "Module.hpp"
#include "TGraph.h"

#include "nlohmann/json.hpp"

namespace yaodaq
{

class Viewer : public Module
{
public:
  Viewer(const std::string& name, const std::string& type,int argc, char** argv): Module(name,type,yaodaq::CLASS::Logger),apps("apps", &argc, argv)
{
  skipConfigFile();
  int x{2540/9};
  int y{1400/4};
  for(std::size_t j=0;j!=4;++j)
  {
    for(std::size_t i=0;i!=9;++i)
    {
      m_TCanvas.push_back(new TCanvas("","",x*i,j*y,x,y));
    }
  }
}
  virtual ~Viewer()
  {
  }
  virtual void onData(const Data& data)
  {
    Json::Value json        = data.getContentAsJson();

  /*m_Event->BoardID        = json["EventInfos"]["BoardID"].asDouble();
  m_Event->EventNumber    = json["EventInfos"]["EventCounter"].asInt();
  m_Event->Pattern        = json["EventInfos"]["Pattern"].asInt();
  m_Event->ChannelMask    = json["EventInfos"]["ChannelMask"].asInt();
  m_Event->EventSize      = json["EventInfos"]["EventSize"].asDouble();
  m_Event->TriggerTimeTag = json["EventInfos"]["TriggerTimeTag"].asDouble();
  m_Event->Period_ns      = json["EventInfos"]["Period_ns"].asDouble();
  m_Event->Model          = json["EventInfos"]["Model"].asString();
  m_Event->FamilyCode     = json["EventInfos"]["FamilyCode"].asString();*/
  for(int i = 0; i != json["Event"]["Groups"].size(); ++i)
  {
    for(int j = 0; j != json["Event"]["Groups"][i]["Channels"].size(); ++j)
    {
      std::string title{"Event "+json["EventInfos"]["EventCounter"].asString()+" Channel "+std::to_string(i*9+j)};
      int nbr_bins{json["Event"]["Groups"][i]["Channels"][j]["Data"].size()};
      double max{json["Event"]["Groups"][i]["Channels"][j]["Data"].size()};
      //TH1F* toto = new TH1F(title.c_str(),title.c_str(),nbr_bins, 0,max);
      TH1F* th = new TH1F(title.c_str(),title.c_str(),nbr_bins, 0,max);
      for(int k = 0; k != json["Event"]["Groups"][i]["Channels"][j]["Data"].size(); ++k)
      {
        th->AddBinContent(k,json["Event"]["Groups"][i]["Channels"][j]["Data"][k].asDouble());
        //channel.Data.push_back(json["Event"]["Groups"][i]["Channels"][j]["Data"][k].asDouble());
      }
      m_TCanvas[i*9+j]->cd();
      m_TCanvas[i*9+j]->Clear();
      if(j==8) th->SetLineColor(kRed);
      th->Draw();
      m_TCanvas[i*9+j]->Modified();
      m_TCanvas[i*9+j]->Update();
      delete th;
    }
  }

  /*for(std::size_t i=0;i!=m_TH1Fs.size();++i)
  {
    c->cd(i+1);
    m_TH1Fs[i]->Draw();
    //c->Modified();
    //c->Update();
  }


  for(std::size_t i=0;i!=m_TH1Fs.size();++i)
  {
    m_TH1Fs[i]->Clear();
    //c->Modified();
    //c->Update();
  }*/

   /*TF1 *f1 = new TF1("f1","sin(x)", -5, 5);
   f1->SetLineColor(kBlue+1);
   f1->SetTitle("My graph;x; sin(x)");
   f1->Draw();*/

   //TRootCanvas *rc = (TRootCanvas *)c->GetCanvasImp();
   //rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
   //delete rc;
   //delete f1;
   //apps.Run();
  }
  bool getApp(){
    start();
    apps.Run();}
private:
  TApplication apps;
  std::vector<TCanvas*> m_TCanvas;
  //std::vector<TH1F*> m_TH1Fs;
};

}

using namespace yaodaq;

int main(int argc, char** argv)
{
  ProgramInfos infos;
  infos.Logo();
  CLI::App  app{"Digitizer"};
  int       port{yaodaq::GeneralParameters::getPort()};
  app.add_option("-p,--port", port, "Port to listen")->check(CLI::Range(0, 65535));
  std::string host{yaodaq::GeneralParameters::getHost()};
  app.add_option("-i,--ip", host, "IP of the server")->check(CLI::ValidIPV4);
  std::string name = "Viewer";
  app.add_option("-n,--name", name, "Name of the mode")
      ->check(
          [](const std::string& t) {
            if(t == "") return "Name is empty";
            else
              return "";
          },
          "Not Empty", "Test is name is empty");
  std::string verbosity{"trace"};
  app.add_option("-v,--verbosity", verbosity, "Verbosity")->check(
    [](const std::string& t) {
      if(t != "off" && t != "trace" && t != "info" && t != "debug" && t != "warning" && t != "critical") return "Wrong verbosity level";
                                                                  else
                                                                    return "";
    },
    "Verbosity level", "Verbosity level");
  try
  {
    app.parse(argc, argv);
  }
  catch(const CLI::ParseError& e)
  {
    spdlog::error("{}", e.what());
    return e.get_exit_code();
  }

  GeneralParameters::setPort(port);
  GeneralParameters::setHost(host);

  Viewer viewer(name,"Default",argc,argv);
  viewer.setVerbosity(verbosity);


  viewer.getApp();
}
