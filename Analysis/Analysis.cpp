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
int main()
{

  // gSystem->Load("/home/sjturpc/DAQ/build/libChannel.so");
 //  gSystem->Load("/home/sjturpc/DAQ/build/libEvent.so");
   TCanvas can;
   //TH1F* histo=new TH1F("Test","Test",1024,0,1024);
 // note that we use "new" to create the TFile and TTree objects, because we
   // want to keep these objects alive when we leave this function.
   TFile fileIn("Toto.root");  
   TTree *t1 = (TTree*)fileIn.Get("Tree");
   Event* event{nullptr};
   t1->SetBranchAddress("Events",&event);
//read all entries and fill the histograms
   Long64_t nentries = t1->GetEntries();
   for (Long64_t i=0;i<nentries;i++) 
   {
     t1->GetEntry(i);
     std::cout<<event->EventNumber<<std::endl;
     for(unsigned int channel=0;channel!=event->Channels.size();++channel)
     {
        double NbrTicks=event->Channels[channel].Data.size();
        double MaxTime=event->Channels[channel].Time[NbrTicks-1];
        TH1F test("Test","Test",NbrTicks,0,MaxTime);
        for(unsigned int j=0;j!=event->Channels[channel].Data.size();++j)
        {
            test.Fill(event->Channels[channel].Time[j],event->Channels[channel].Data[j]);
        }
        test.SetMarkerStyle(kFullSquare);
        test.Draw("*H HIST");
        can.SaveAs((std::to_string(i)+"_"+std::to_string(channel)+".pdf").c_str());
        test.Clear();
     }
     event->clear();
  }

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








