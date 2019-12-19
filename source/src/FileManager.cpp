#include "FileManager.hpp"
#include "SHA512.hpp"
#include <iostream>
#include <sstream>

void FileManager::Init(const std::string& filename, uint16_t EnableMask,int nbrChannels, double xinc) 
{
    fname = filename;
    setNbrChannels(nbrChannels);
    setTick(xinc);
}

void FileManager::CloseFile() {
  f->Write();
  f->Close();
  //m_SHA512=sw::sha512::file(finalFilename);
  delete event;
}

std::string FileManager::SHA512()
{
	return m_SHA512;
}

void FileManager::AddEvents()
{
      addEvent();
}


void FileManager::OpenFile() {
  finalFilename=path + "/" + fname+".root";
  f = new TFile(finalFilename.c_str(), "RECREATE",fname.c_str());
  t = new TTree("Tree", "Tree");
  event = new Event();
  t->Branch("Events", &event,10,0);
 
}
void FileManager::addEvent16() {
  /*BoardID = dat.EventInfo.BoardId;
  EventNumber = dat.EventInfo.EventCounter;
  Pattern = dat.EventInfo.Pattern & 0xFFFF;
  for (std::size_t ch = 0; ch < dat.WDcfg.Nch; ch++) {
    Channel chan;
    chan.RecordLength = dat.Event16->ChSize[ch];
    chan.Number = ch;
    chan.Group = 0;
    chan.Name = "Channel" + std::to_string(ch);
    chan.TriggerTimeTag = dat.EventInfo.TriggerTimeTag;
    chan.DCoffset = dat.WDcfg.DCoffset[ch] & 0xFFFF;
    chan.StartIndexCell = 0;
    if (chan.RecordLength <= 0) {
      Channels[ch] = chan;
    } else {
      chan.Data.reserve(chan.RecordLength);
      for (std::size_t i = 0; i != chan.RecordLength; ++i) {
        chan.Data.push_back(dat.Event16->DataChannel[ch][i]);
        // std::cout<<chan.Data[i]<<std::endl;
      }

      // chan.Data.assign(Event.DataGroup[gr].DataChannel[ch][0],Event.DataGroup[gr].DataChannel[ch][int(chan.RecordLength-1)]);
      Channels[ch] = chan;
    }
    
  }
  t->Fill();*/
}

void FileManager::addEvent8() {
 /* BoardID = dat.EventInfo.BoardId;
  EventNumber = dat.EventInfo.EventCounter;
  Pattern = dat.EventInfo.Pattern & 0xFFFF;
  for (std::size_t ch = 0; ch < dat.WDcfg.Nch; ch++) {
    Channel chan;
    chan.RecordLength = dat.Event8->ChSize[ch];
    chan.Number = ch;
    chan.Group = 0;
    chan.Name = "Channel" + std::to_string(ch);
    chan.TriggerTimeTag = dat.EventInfo.TriggerTimeTag;
    // chan.DCoffset=WDcfg.DCoffset[ch] & 0xFFFF;
    chan.StartIndexCell = 0;
    if (chan.RecordLength <= 0) {
      Channels[ch] = chan;
    } else {
      chan.Data.reserve(chan.RecordLength);
      for (std::size_t i = 0; i != chan.RecordLength; ++i) {
        chan.Data.push_back(dat.Event8->DataChannel[ch][i]);
        // std::cout<<chan.Data[i]<<std::endl;
      }

      // chan.Data.assign(Event.DataGroup[gr].DataChannel[ch][0],Event.DataGroup[gr].DataChannel[ch][int(chan.RecordLength-1)]);
      Channels[ch] = chan;
    }
    
  }
  t->Fill();*/
}

void FileManager::addEvent742() 
{
  Channel chan;
  event->BoardID=dat.EventInfo.BoardId;
  event->EventNumber = dat.EventInfo.EventCounter;
  event->Pattern = dat.EventInfo.Pattern & 0xFFFF;
  for (std::size_t gr = 0; gr < (nbrChannels / 8); gr++) 
  {
    if (dat.Event742->GrPresent[gr]) 
    {
      for (std::size_t ch = 0; ch < 9; ch++) 
      {
        chan.clear();
        chan.RecordLength = dat.Event742->DataGroup[gr].ChSize[ch];
        chan.Number = gr * 9 + ch;
        chan.Group = gr;
        if ((gr * 9 + ch) == 8 || (gr * 9 + ch) == 17 || (gr * 9 + ch) == 26 ||(gr * 9 + ch) == 35)chan.Name = "Trigger" + std::to_string(gr);
        else chan.Name = "Channel" + std::to_string((gr * 8) + ch);
        chan.TriggerTimeTag = dat.Event742->DataGroup[gr].TriggerTimeTag;
        chan.DCoffset = dat.WDcfg.DCoffset[ch] & 0xFFFF;
        chan.StartIndexCell = dat.Event742->DataGroup[gr].StartIndexCell;
        if (chan.RecordLength >0) 
        {
          for (std::size_t i = 0; i != chan.RecordLength; ++i) 
          {
            chan.Data.push_back(dat.Event742->DataGroup[gr].DataChannel[ch][i]);
            chan.Time.push_back(dat.WDcfg.Ts*i*1.0e-9);
            //std::cout<<chan.Data[i]<<std::endl;
          }
          // chan.Data.assign(Event.DataGroup[gr].DataChannel[ch][0],Event.DataGroup[gr].DataChannel[ch][int(chan.RecordLength-1)]);
        }
        event->addChannel(chan);
        chan.clear();
      }
    }
  }
  t->Fill();
  event->clear();
}
