#include "FileManager.hpp"

void FileManager::Init(const std::string filename, uint16_t EnableMask) {
  fname = filename;
}

void FileManager::CloseFile() {
  t->Write();
  f->Close();
}



void FileManager::AddEvents()
{




    /* Write Event data to file */
    if (dat.WDrun.ContinuousWrite || dat.WDrun.SingleWrite) 
	{
      addEvent();
      if (dat.WDrun.SingleWrite) {
        std::cout << "Single Event saved to output files" << std::endl;
        dat.WDrun.SingleWrite = 0;
      }
    }



}


void FileManager::OpenFile() {
  f = new TFile((path + "/" + fname).c_str(), "RECREATE", "File", 9);
  t = new TTree("Toto", "Toto");
  t->Branch("BoardId", &BoardID);
  t->Branch("EventNumber", &EventNumber);
  t->Branch("Pattern", &Pattern);
  t->Branch("Channels", &Channels);
}

void FileManager::addEvent16() {
  BoardID = dat.EventInfo.BoardId;
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
      continue;
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
  t->Fill();
}

void FileManager::addEvent8() {
  BoardID = dat.EventInfo.BoardId;
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
      continue;
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
  t->Fill();
}

void FileManager::addEvent742() {
  BoardID = dat.EventInfo.BoardId;
  EventNumber = dat.EventInfo.EventCounter;
  Pattern = dat.EventInfo.Pattern & 0xFFFF;
  for (std::size_t gr = 0; gr < (nbrChannels / 8); gr++) {
    if (dat.Event742->GrPresent[gr]) {
      for (std::size_t ch = 0; ch < 9; ch++) {
        Channel chan;
        chan.RecordLength = dat.Event742->DataGroup[gr].ChSize[ch];
        chan.Number = gr * 9 + ch;
        chan.Group = gr;
        if ((gr * 9 + ch) == 8 || (gr * 9 + ch) == 17 || (gr * 9 + ch) == 26 ||
            (gr * 9 + ch) == 35)
          chan.Name = "Trigger" + std::to_string(gr);
        else
          chan.Name = "Channel" + std::to_string((gr * 8) + ch);
        chan.TriggerTimeTag = dat.Event742->DataGroup[gr].TriggerTimeTag;
        chan.DCoffset = dat.WDcfg.DCoffset[ch] & 0xFFFF;
        chan.StartIndexCell = dat.Event742->DataGroup[gr].StartIndexCell;
        if (chan.RecordLength <= 0) {
          Channels[(gr * 9 + ch)] = chan;
          continue;
        } else {
          chan.Data.reserve(chan.RecordLength);
          for (std::size_t i = 0; i != chan.RecordLength; ++i) {
            chan.Data.push_back(dat.Event742->DataGroup[gr].DataChannel[ch][i]);
            // std::cout<<chan.Data[i]<<std::endl;
          }

          // chan.Data.assign(Event.DataGroup[gr].DataChannel[ch][0],Event.DataGroup[gr].DataChannel[ch][int(chan.RecordLength-1)]);
          Channels[(gr * 9 + ch)] = chan;
        }
      }
    }
  }
  t->Fill();
}
