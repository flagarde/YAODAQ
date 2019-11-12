#include "Channel.hpp"
#include "Event.hpp"
#include <iostream>
#include "TTree.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TH1F.h"
#include <vector>
#include "TTreeReader.h"
#include "TSpectrum.h"

int main()
{

  // gSystem->Load("/home/sjturpc/DAQ/build/libChannel.so");
 //  gSystem->Load("/home/sjturpc/DAQ/build/libEvent.so");
   TCanvas can;
   TSpectrum spect;
   //TH1F* histo=new TH1F("Test","Test",1024,0,1024);
 // note that we use "new" to create the TFile and TTree objects, because we
   // want to keep these objects alive when we leave this function.
   TFile fileIn("Toto.root");  
   TTree *t1 = (TTree*)fileIn.Get("Tree");
   Event* event{nullptr};
   t1->SetBranchAddress("Events",&event);
//read all entries and fill the histograms
   Long64_t nentries = t1->GetEntries();
   TH1F test("","",1024,0,1024);
   TH1F all("","",1024,0,1024);
   TH1F histos("","",4096,-1.0,1.0);
   int  channel_0_seen{0};
    int channel_1_seen{0};
   int both_seen{0};
   for (Long64_t i=0;i<nentries;i++) 
   {
       event->clear();
     t1->GetEntry(i);
     
     double NbrTicks=event->Channels[0].Data.size();
     bool channel_0_saw_something{false};
     bool channel_1_saw_something{false};
     for(unsigned int channel=0;channel!=2;++channel)
     {
         test.SetTitle(("Event"+std::to_string(i)+"_Channel"+std::to_string(channel)+".pdf").c_str());
         test.SetName(("Event"+std::to_string(i)+"_Channel"+std::to_string(channel)+".pdf").c_str());
           all.SetTitle(("Event"+std::to_string(i)+"_Channel"+std::to_string(channel)+".pdf").c_str());
         all.SetName(("Event"+std::to_string(i)+"_Channel"+std::to_string(channel)+".pdf").c_str());
         test.Reset();
         all.Reset();
        double min{999999};
        double max{-99999};
        double mean{0};
        double sigma{0};
        double MaxTime=event->Channels[channel].Time[NbrTicks-1];
        
        for(unsigned int j=0;j!=event->Channels[channel].Data.size();++j)
        { 
            double scaled=event->Channels[channel].Data[j]*1.0/4096;
            if(scaled<min)min=scaled;
            else if (scaled>max) max=scaled;
            mean+=scaled;
        }
        histos.SetTitle(("Histos_Event"+std::to_string(i)+"_Channel"+std::to_string(channel)+".pdf").c_str());
        histos.SetName(("Histos_Event"+std::to_string(i)+"_Channel"+std::to_string(channel)+".pdf").c_str());
        mean/=(event->Channels[channel].Data.size());
        for(unsigned int j=0;j!=event->Channels[channel].Data.size();++j)
        {
            sigma+=(event->Channels[channel].Data[j]*1.0/4096-mean)*(event->Channels[channel].Data[j]*1.0/4096-mean);
            histos.Fill(event->Channels[channel].Data[j]*1.0/4096);
        }
        sigma=std::sqrt(sigma/((event->Channels[channel].Data.size())-1));
        
        //std::cout<<"MEAN : "<<mean<<" VARIANCE : "<<sigma<<std::endl;
        histos.GetXaxis()->SetRangeUser(mean-20*sigma,mean+20*sigma);
        
        double mean_abs{0};
        double number{0};
        for(unsigned int j=0;j!=event->Channels[channel].Data.size();++j)
        {
            if(channel==0&&((event->Channels[channel].Data[j]*1.0/4096)-mean)<0)
            {
                number++;
                mean_abs+=event->Channels[channel].Data[j]*1.0/4096;
            }
            else if(channel==1&&((event->Channels[channel].Data[j]*1.0/4096)-mean)>0)
            {
                number++;
                mean_abs+=event->Channels[channel].Data[j]*1.0/4096;
            }
        }
        mean_abs/=1.0*number;
        double sigma_abs{0};
        for(unsigned int j=0;j!=event->Channels[channel].Data.size();++j)
        {
            if(channel==0&&((event->Channels[channel].Data[j]*1.0/4096)-mean)<0)
            {
                sigma_abs+=(event->Channels[channel].Data[j]*1.0/4096-mean_abs)*(event->Channels[channel].Data[j]*1.0/4096-mean_abs);
            }
            else if(channel==1&&((event->Channels[channel].Data[j]*1.0/4096)-mean)>0)
            {
                sigma_abs+=(event->Channels[channel].Data[j]*1.0/4096-mean_abs)*(event->Channels[channel].Data[j]*1.0/4096-mean_abs);
            }
        }
        sigma_abs=std::sqrt(sigma_abs/(number-1));
        
        
        for(unsigned int j=0;j!=event->Channels[channel].Data.size();++j)
        {
            double meaned=event->Channels[channel].Data[j]*1.0/4096-mean_abs;
            if(std::fabs(meaned)>=0.5*sigma_abs&&channel==0&&event->Channels[channel].Data[j]*1.0/4096-mean_abs<-0.001&&(j>200&&j<250)) test.Fill(j,event->Channels[channel].Data[j]*1.0/4096-mean_abs);
            else if(std::fabs(meaned)>=0.5*sigma_abs&&channel==1&&event->Channels[channel].Data[j]*1.0/4096-mean_abs>0.001&&(j>200&&j<250))  test.Fill(j,event->Channels[channel].Data[j]*1.0/4096-mean_abs);
            else test.Fill(j,0);
            all.Fill(j,event->Channels[channel].Data[j]*1.0/4096);
            test.GetYaxis()->SetRangeUser(min-mean_abs,max-mean_abs);
            all.GetYaxis()->SetRangeUser(min-((max-min)/20.0),max+((max-min)/20.0));
            all.SetLineColor(kRed);
            //test.SetLabelColor(kRed);
        }
        double founded{0};
        for(unsigned int j=0;j!=test.GetNbinsX();++j)
        {
            if(test.GetBinContent(j)!=0) founded++;
        }
        
        if(founded>3)
        {
            std::cout<<event->EventNumber<<std::endl;
            std::cout<<"Founded "<<founded<<" bins "<<std::endl;
            if(channel==0)
            {
                channel_0_seen++;
                channel_0_saw_something=true;
            }
            else if (channel==1)
            {
                channel_1_seen++;
                channel_1_saw_something=true;
            }
            
        }
        
       // test.SetMarkerStyle(kFullSquare);
        can.Divide(1,2);
        can.cd(1);
        all.Draw("HIST");
      can.cd(2);
        
     //   test.Fit("gaus","","",200,250);
        test.Draw("HIST");
        //test.Smooth(2);
       // spect.Search(&test);
      //  std::cout<<spect.GetNPeaks()<<" picks founded "<<std::endl;
         if(founded>3) can.SaveAs(("Found_Event"+std::to_string(i)+"_Channel"+std::to_string(channel)+".pdf").c_str());
      //   else can.SaveAs(("Event"+std::to_string(i)+"_Channel"+std::to_string(channel)+".pdf").c_str());
        can.Divide(1,1);
        can.cd(1);
        histos.Draw("HIST");
      //  if(founded) can.SaveAs(("Found/Histos_Event"+std::to_string(i)+"_Channel"+std::to_string(channel)+".pdf").c_str());
      //  else can.SaveAs(("Histos_Event"+std::to_string(i)+"_Channel"+std::to_string(channel)+".pdf").c_str());
        test.Clear();
        histos.Clear();
        all.Clear();
        can.Clear();
     }
     if(channel_0_saw_something==true&&channel_1_saw_something==true) both_seen++;
  }
  fileIn.Close();
  
  std::cout<<nentries<<"  "<<(nentries*20/400)<<std::endl;
  std::cout<<channel_0_seen<<"  "<<channel_1_seen<<"  " <<both_seen<<std::endl;
  std::cout<<"Efficiency channel 0 : "<<channel_0_seen*100.0/(nentries*20/400)<<" Efficiency channel 1 : "<<channel_1_seen*100.0/(nentries*20/400)<< " Efficiency both 0&&1 : "<<both_seen*100.0/(nentries*20/400)<<std::endl;
}
   
   
   
   
   
   
   /*TTree *t4 = (TTree*)f->Get("Toto");
   Event event;
   // get two branches and set the branch address
   TBranch *branch = t4->GetBranch("Events");
   branch->SetAddress(&event);
   Int_t nevent = t4->GetEntries();
   for (Int_t i=0; i<nevent; i++) 
   {
      //read branch "fNtrack"only
      branch->GetEntry(i);
      std::cout<<event.EventNumber<<std::endl;

     // clear tracks array
     histo->SetMarkerStyle(kFullSquare);
     histo->Draw("*H HIST");
     can.SaveAs((std::to_string(i)+".pdf").c_str());
     histo->Clear();
     event.clear();
   }*/








