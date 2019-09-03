#include "Channel.hpp"
#include <iostream>
#include "TTree.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"
#include <vector>

int main()
{
    TCanvas can;
   TH1F* histo=new TH1F("Test","Test",1024,0,1024);
 // note that we use "new" to create the TFile and TTree objects, because we
   // want to keep these objects alive when we leave this function.
   TFile *f = new TFile("./Toto.root");
   TTree *t4 = (TTree*)f->Get("Toto");

   // create a pointer to an event object for reading the branch values.
   std::vector<Channel>* event = new std::vector<Channel>();
   double eventNbr;
   // get two branches and set the branch address
   TBranch *bntrack = t4->GetBranch("EventNumber");
   TBranch *branch  = t4->GetBranch("Channels");
   branch->SetAddress(&event);
   bntrack->SetAddress(&eventNbr);
   Int_t nevent = t4->GetEntries();
   Int_t nselected = 0;
   Int_t nb = 0;
   for (Int_t i=0; i<nevent; i++) 
   {
      //read branch "fNtrack"only
      branch->GetEntry(i);
      
      std::cout<<event->at(0).Name<<std::endl;
      std::cout<<event->at(0).Data.size()<<std::endl;
      for(unsigned int i=0;i!=event->at(0).Data.size();++i)
      {
	histo->Fill(i,event->at(0).Data[i]);
	//std::cout<<event->at(0).Data[i]<<std::endl;
      }
     // clear tracks array
     histo->SetMarkerStyle(kFullSquare);
     histo->Draw("*H HIST");
     can.SaveAs((std::to_string(i)+".pdf").c_str());
     histo->Clear();
     event->clear();
   }
	

}







