#include "Channel.hpp"
#include "Event.hpp"
#include <iostream>
#include "TTree.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"
#include <vector>
#include "TTreeReader.h"
int main()
{
   TCanvas can;
   TH1F* histo=new TH1F("Test","Test",1024,0,1024);
 // note that we use "new" to create the TFile and TTree objects, because we
   // want to keep these objects alive when we leave this function.
   TFile fileIn("Toto.root");  
   TTreeReader theReader("Toto",&fileIn);
   TTreeReaderValue<Event> eventRV(theReader, "Events");
   
   while(theReader.Next())
   {

      auto event = eventRV.Get();      //Read complete accepted event
                                       //in memory.
     std::cout<<event->EventNumber<<std::endl;
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








