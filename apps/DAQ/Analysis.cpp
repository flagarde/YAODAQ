#include "Channel.hpp"
#include "Event.hpp"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TROOT.h"
#include "TSpectrum.h"
#include "TTree.h"
#include "TTreeReader.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

#include "CLI.hpp"

int NbrEventToProcess(int& nbrEvents, const Long64_t& nentries)
{
  if(nbrEvents == 0) return nentries;
  else if(nbrEvents > nentries)
  {
    std::cout << "WARNING : You ask to process " << nbrEvents << " but this run only have " << nentries << " !!!";
    return nbrEvents = nentries;
  }
  else return nbrEvents;
}

class PositiveNegative
{
public:
  PositiveNegative() {}
  PositiveNegative(const std::string& PN) { Parse(PN); }
  bool        IsPositiveSignal() { return m_positive; }
  std::string asString()
  {
    if(m_positive) return "POSITIVE";
    else
      return "NEGATIVE";
  }

private:
  void Parse(const std::string& PN)
  {
    if(PN == "Positive" || PN == "POSITIVE" || PN == "P" || PN == "+") m_positive = true;
    else if(PN == "Negative" || PN == "NEGATIVE" || PN == "N" || PN == "-")
      m_positive = false;
    else
    {
      std::cout << "BAD argument !!!" << std::endl;
      std::exit(-5);
    }
  }
  bool m_positive{false};
};

class channel
{
public:
  channel(const int& chn, const std::string& PN): m_channelNumber(chn), m_PN(PN) {}
  channel(){};
  int         getChannelNumber() { return m_channelNumber; }
  bool        isPositive() { return m_PN.IsPositiveSignal(); }
  bool        isNegative() { return !m_PN.IsPositiveSignal(); }
  std::string PNasString() { return m_PN.asString(); }

private:
  int              m_channelNumber{-1};
  PositiveNegative m_PN;
};

class Channels
{
public:
  std::size_t getNumberOfChannelActivated() { return m_Channel.size(); }
  void        activateChannel(const int& chn, const std::string& PN) { m_Channel.emplace(chn, channel(chn, PN)); }
  void        print()
  {
    std::cout << "Channels ENABLED for the analysis : \n";
    for(std::map<int, channel>::iterator it = m_Channel.begin(); it != m_Channel.end(); ++it)
    { std::cout << "\t--> Channel " << it->first << " is declared to have " << it->second.PNasString() << " signal" << std::endl; }
  }
  bool DontAnalyseIt(const int& channel)
  {
    if(m_Channel.find(channel) == m_Channel.end()) return true;
    else
      return false;
  }
  bool ShouldBePositive(const int& ch) { return m_Channel[ch].isPositive(); }

private:
  std::map<int, channel> m_Channel;
};

TH1D CreateAndFillWaveform(const int& eventNbr, const Channel& channel, const std::string& name = "", const std::string title = "")
{
  std::string my_name  = name + "_Event_" + std::to_string(eventNbr) + "_Channel_" + std::to_string(int(channel.Number));
  std::string my_title = title + " Event " + std::to_string(eventNbr) + " Channel " + std::to_string(int(channel.Number));
  TH1D        th1(my_title.c_str(), my_name.c_str(), channel.Data.size(), 0, channel.Data.size());
  for(std::size_t i = 0; i != channel.Data.size(); ++i) th1.Fill(i, channel.Data[i]);
  auto                      result = std::minmax_element(channel.Data.begin(), channel.Data.end());
  std::pair<double, double> minmax((*result.first), (*result.second));
  th1.GetYaxis()->SetRangeUser((minmax.first - ((minmax.second - minmax.first)) / 20.0), (minmax.second + ((minmax.second - minmax.first)) / 20.0));
  th1.SetLineColor(kBlue);
  return std::move(th1);
}

std::pair<std::pair<double, double>, std::pair<double, double>> MeanSTD(const Channel& channel, const std::pair<double, double>& window = std::pair<double, double>{99999999, -999999})
{
  double meanwindows{0};
  double sigmawindows{0};
  int    binusedwindows{0};
  double meannoise{0};
  double sigmanoise{0};
  int    binusednoise{0};
  for(std::size_t j = 0; j != channel.Data.size(); ++j)
  {
    /////FIX THIS
    if(channel.Data[j] == 0) continue;
    ////////////////////////////////
    if(j <= window.first || j >= window.second)
    {
      binusednoise++;
      meannoise += channel.Data[j];
    }
    else
    {
      binusedwindows++;
      meanwindows += channel.Data[j];
    }
  }
  meannoise /= binusednoise;
  meanwindows /= binusedwindows;
  for(std::size_t j = 0; j != channel.Data.size(); ++j)
  {
    /////FIX THIS
    if(channel.Data[j] == 0) continue;
    ////////////////////////////////
    if(j <= window.first || j >= window.second) { sigmanoise += (channel.Data[j] - meannoise) * (channel.Data[j] - meannoise); }
    else
    {
      sigmawindows += (channel.Data[j] - meanwindows) * (channel.Data[j] - meanwindows);
    }
  }
  sigmanoise   = std::sqrt(sigmanoise / (binusednoise - 1));
  sigmawindows = std::sqrt(sigmawindows / (binusedwindows - 1));
  std::pair<double, double> noise(meannoise, sigmanoise);
  std::pair<double, double> signal(meanwindows, sigmawindows);
  return std::pair<std::pair<double, double>, std::pair<double, double>>(noise, signal);
}

class Chamber
{
public:
  void activateChannel(const int& chn, const std::string& PN)
  {
    m_Channels.activateChannel(chn,PN);
  }
  double getEfficiency()
  {
    return m_NumberFired*1.0/m_Total;
  }
  double getMultiplicity()
  {
    return m_TotalHit*1.0/m_NumberFired;
  }
private:
  int m_TotalHit{0};
  int m_NumberFired{0};
  int m_Total{0};
  Channels m_Channels;
};

/*
TH1D CreateSelectionPlot(const TH1D& th)
{
    TH1D selected=th;
    selected.SetLineColor(kRed);
    std::string name="Selected_"+std::string(th.GetName());
    std::string title="Selected "+std::string(th.GetTitle());
    selected.SetTitle(title.c_str());
    selected.SetName(name.c_str());
    return std::move(selected);
}*/

//####FIX ME PUT THIS AS PARAMETER OUT OF THE PROGRAM ######

int main(int argc, char** argv)
{
  CLI::App app{"Analysis"};
  std::string file{""};
  app.add_option("-f,--file",file, "Name of the file to process")->required()->check(CLI::ExistingFile);
  int NbrEvents{0};
  app.add_option("-n,--events",NbrEvents,"Number of event to process",0)->check(CLI::PositiveNumber);
  std::string nameTree{"Tree"};
  app.add_option("-t,--tree",nameTree,"Name of the TTree","Tree");
  std::pair<double,double> SignalWindow;
  app.add_option("-w,--window",SignalWindow,"Signal window")->required()->type_size(2);
  int NumberChambers{0};
  app.add_option("-c,--chambers",NumberChambers,"Number of chamber(s)")->check(CLI::PositiveNumber)->required();
  std::vector<int> distribution;
  app.add_option("-d,--distribution",distribution,"Channel is in wich chamber start at 0 and -1 if not connected")->required();
  std::vector<std::string> polarity;
  app.add_option("-p,--polarity",polarity,"Polarity of the signal Positive,+,Negative,-")->required();
  try 
  {
    app.parse(argc,argv);
  } 
  catch(const CLI::ParseError &e) 
  {
    return app.exit(e);
  }
  //Open The file
  TFile fileIn(file.c_str());
  if(fileIn.IsZombie())
  {
    std::cout << "File Not Opened" << std::endl;
    std::exit(-3);
  }
  TTree* Run = static_cast<TTree*>(fileIn.Get(nameTree.c_str()));
  if(Run == nullptr || Run->IsZombie())
  {
    std::cout << "Problem Opening TTree \"Tree\" !!!" << std::endl;
    std::exit(-4);
  }
  
  TH1D                      sigmas_event("Ration_event", "Ration_event", 100, 0, 5);
  TH1D                      sigmas_noise("Ration_event", "Ration_event", 100, 0, 5);
  double                    scalefactor = (4.2 * 20) / (20 * 20);
  Channels                  channels;
  channels.activateChannel(0, "N");
  channels.activateChannel(1, "N");
  
  


  Long64_t NEntries = Run->GetEntries();
  NbrEvents         = NbrEventToProcess(NbrEvents, NEntries);
  channels.print();
  Event* event{nullptr};
  if(Run->SetBranchAddress("Events", &event))
  {
    std::cout << "Error while SetBranchAddress !!!" << std::endl;
    std::exit(-5);
  }

  TCanvas can;
  // std::vector<TH1D> Verif;
  std::map<int, int> Efficiency;
  for(Long64_t evt = 0; evt < NbrEvents; ++evt)
  {
    event->clear();
    Run->GetEntry(evt);
    for(unsigned int ch = 0; ch != event->Channels.size(); ++ch)
    {
      if(channels.DontAnalyseIt(ch))
        continue;  // Data for channel X is in file but i dont give a *** to
                   // analyse it !
      if(evt == 0) Efficiency[ch] = 0;
      TH1D                                                            waveform = CreateAndFillWaveform(evt, event->Channels[ch], "Waveform", "Waveform");
      std::pair<std::pair<double, double>, std::pair<double, double>> meanstd  = MeanSTD(event->Channels[ch], SignalWindow);
      // std::cout<<"Event "<<evt<<" Channel "<<ch<<"/n";
      // std::cout<<" Mean : "<<meanstd.first<<" STD :
      // "<<meanstd.second<<std::endl;
      // selected with be updated each time we make some selection... For now
      // it's the same as Waveform one but in Red !!!
      // TH1D selected = CreateSelectionPlot(waveform);
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // 0
      // Substract the mean value of amplitudes ( center to 0 )
      for(std::size_t i = 0; i != event->Channels[ch].Data.size(); ++i) { event->Channels[ch].Data[i] = (event->Channels[ch].Data[i]) - meanstd.first.first; }
      TH1D after0 = CreateAndFillWaveform(evt, event->Channels[ch], "Step_0", "Step 0");

      // Check the mean if 0
      std::pair<std::pair<double, double>, std::pair<double, double>> testmeanstd = MeanSTD(event->Channels[ch]);
      // assert(testmeanstd.first.first==0);
      testmeanstd = MeanSTD(event->Channels[ch], SignalWindow);
      std::vector<TH1D> IterationPlots;
      int               Iter{0};
      bool              ImEvent{false};

      while(1) /*for(std::size_t Iter=0;Iter!=10;++Iter)*/
      {
        int NbrBinStillHere{0};
        int NbrBinStillHereInSignalRegions{0};
        // Part 1
        // Calculate the mean/std for positive part if signal should be POSITIVE
        // or the negative one if signal should be NEGATIVE
        for(std::size_t i = 0; i != event->Channels[ch].Data.size(); ++i)
        {
          if(channels.ShouldBePositive(ch) == true && event->Channels[ch].Data[i] > 0)
          {
            if(i > SignalWindow.first && i < SignalWindow.second) NbrBinStillHereInSignalRegions++;
            NbrBinStillHere++;
          }
          else if(channels.ShouldBePositive(ch) == false && event->Channels[ch].Data[i] < 0)
          {
            NbrBinStillHere++;
            if(i > SignalWindow.first && i < SignalWindow.second) NbrBinStillHereInSignalRegions++;
          }
          else
            event->Channels[ch].Data[i] = 0;
        }
        std::string name  = "Iter_" + std::to_string(2 * Iter + 1);
        std::string title = "Iter " + std::to_string(2 * Iter + 1);
        IterationPlots.push_back(CreateAndFillWaveform(evt, event->Channels[ch], name.c_str(), title.c_str()));
        if(NbrBinStillHereInSignalRegions == 0)
        {
          ImEvent = false;
          break;
        }
        else if(NbrBinStillHere == NbrBinStillHereInSignalRegions)
        {
          ImEvent = true;
          Efficiency[ch]++;
          break;
        }
        else if(NbrBinStillHere == 1)
          ;
        MeanSTD(event->Channels[ch]);
        std::pair<std::pair<double, double>, std::pair<double, double>> meanstd = MeanSTD(event->Channels[ch] /*,SignalWindow*/);
        // std::cout<<"Iteration "<<Iter<<" Mean  : "<<meanstd.first<<" STD :
        // "<<meanstd.second<<std::endl;
        // Part 2
        // Realigne to the new mean
        name  = "Iter_" + std::to_string(2 * Iter + 2);
        title = "Iter " + std::to_string(2 * Iter + 1);
        for(std::size_t i = 0; i != event->Channels[ch].Data.size(); ++i)
        {
          if(event->Channels[ch].Data[i] == 0) event->Channels[ch].Data[i] = 0;
          else
            event->Channels[ch].Data[i] = event->Channels[ch].Data[i] - meanstd.first.first;
        }
        IterationPlots.push_back(CreateAndFillWaveform(evt, event->Channels[ch], name.c_str(), title.c_str()));
        Iter++;
      }

      if(ImEvent == true)
      {
        waveform.SetLineColor(kGreen);
        std::cout << "EVENT   " << testmeanstd.first.first << "   " << testmeanstd.first.second << "   " << testmeanstd.second.first << "   " << testmeanstd.second.second << "  "
                  << testmeanstd.second.second / testmeanstd.first.second << std::endl;
        sigmas_event.Fill(testmeanstd.second.second);
      }
      else
      {
        waveform.SetLineColor(kRed);
        std::cout << testmeanstd.first.first << "   " << testmeanstd.first.second << "   " << testmeanstd.second.first << "   " << testmeanstd.second.second << std::endl;
        sigmas_noise.Fill(testmeanstd.second.second);
      }
      // Verif.push_back(waveform);

      can.Clear();
      if(ImEvent == true)
      {
        can.Clear();
        waveform.GetXaxis()->SetRangeUser(300, 370);
        waveform.Scale(1.0 / 4096);
        waveform.Draw("HIST");
        // if(channels.ShouldBePositive(ch)) waveform.Fit(f1);
        // else waveform.Fit(f1);
        // can.SaveAs(("GOOD/GOOD"+std::to_string(evt)+"_Channel"+std::to_string(ch)+".pdf").c_str());
      }

      can.Clear();
      can.Divide(5, 5);
      can.cd(1);

      waveform.Draw("HIST");
      can.cd(2);
      if(ImEvent == true) after0.SetLineColor(kGreen);
      else
        after0.SetLineColor(kRed);
      after0.Draw("HIST");
      for(std::size_t Iter = 0; Iter != IterationPlots.size(); ++Iter)
      {
        if(ImEvent == true) IterationPlots[Iter].SetLineColor(kGreen);
        else
          IterationPlots[Iter].SetLineColor(kRed);
        can.cd(Iter + 3);
        IterationPlots[Iter].Draw("HIST");
      }
      // can.SaveAs(("Compare_Event"+std::to_string(evt)+"_Channel"+std::to_string(ch)+".pdf").c_str());
    }
    /* if(Verif.size()==16)
     {
         static int j=0;
         can.Clear();
         can.Divide(4,4);
         for(std::size_t i=0;i!=Verif.size();++i)
         {
             can.cd(i+1);
             Verif[i].Draw("HIST");
         }
         can.SaveAs((std::to_string(j)+".pdf").c_str());
         Verif.clear();
         j++;
         can.Clear();
         sigmas_noise.Draw("HIST");
         sigmas_event.Draw("HIST SAME");
         sigmas_event.SetLineColor(kGreen);
         can.SaveAs(("Ratio"+std::to_string(j)+".pdf").c_str());
     }*/

    std::cout << "*******************************************************************" << std::endl;
    for(std::map<int, int>::iterator it = Efficiency.begin(); it != Efficiency.end(); ++it)
    { std::cout << "NUMBER EVENT " << it->second << " TOTAL EVENT " << evt << " EFFICIENCY CHANNEL " << it->first << " : " << (it->second * 100.0) / (evt * scalefactor) << " % " << std::endl; }
    std::cout << "*******************************************************************" << std::endl;
  }
  if(event != nullptr) delete event;
  if(Run != nullptr) delete Run;
  if(fileIn.IsOpen()) fileIn.Close();
  std::cout << "BYE !!!" << std::endl;
}
