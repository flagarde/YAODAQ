#include "CLI/CLI.hpp"
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
/*
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
  int         getPolarity()
  {
    if (isPositive()) return +1;
    else return -1;
  }
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
  int getPolarity(const int& ch)
  {
    return m_Channel[ch].getPolarity();
  }

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

std::pair<std::pair<double, double>, std::pair<double, double>> MeanSTD(const Channel& channel, const std::pair<double, double>& window_signal = std::pair<double, double>{99999999, -999999},
                                                                        const std::pair<double, double>& window_noise = std::pair<double, double>{99999999, -999999})
{
  double meanwindows{0};
  double sigmawindows{0};
  int    binusedwindows{0};
  double meannoise{0};
  double sigmanoise{0};
  int    binusednoise{0};
  for(std::size_t j = 0; j != channel.Data.size(); ++j)
  {
    ////////////////////////////////
    if(j >= window_noise.first && j <= window_noise.second)
    {
      binusednoise++;
      meannoise += channel.Data[j];
    }
    else if(j >= window_signal.first && j <= window_signal.second)
    {
      binusedwindows++;
      meanwindows += channel.Data[j];
    }
  }
  meannoise /= binusednoise;
  meanwindows /= binusedwindows;
  for(std::size_t j = 0; j != channel.Data.size(); ++j)
  {
    if(j >= window_noise.first && j <= window_noise.second) { sigmanoise += (channel.Data[j] - meannoise) * (channel.Data[j] - meannoise); }
    else if(j >= window_signal.first && j <= window_signal.second)
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
  void   activateChannel(const int& chn, const std::string& PN) { m_Channels.activateChannel(chn, PN); }
  double getEfficiency() { return m_NumberFired * 1.0 / m_Total; }
  double getMultiplicity() { return m_TotalHit * 1.0 / m_NumberFired; }

private:
  int      m_TotalHit{0};
  int      m_NumberFired{0};
  int      m_Total{0};
  Channels m_Channels;
};*/

/*
 * TH1D CreateSelectionPlot(const TH1D& th)
 * {
 *    TH1D selected=th;
 *    selected.SetLineColor(kRed);
 *    std::string name="Selected_"+std::string(th.GetName());
 *    std::string title="Selected "+std::string(th.GetTitle());
 *    selected.SetTitle(title.c_str());
 *    selected.SetName(name.c_str());
 *    return std::move(selected);
 * }*/

//####FIX ME PUT THIS AS PARAMETER OUT OF THE PROGRAM ######

int main(int argc, char** argv)
{
  CLI::App    app{"Analysis"};
  std::string file{""};
  app.add_option("-f,--file", file, "Name of the file to process")->required()->check(CLI::ExistingFile);
  int NbrEvents{0};
  app.add_option("-e,--events", NbrEvents, "Number of event to process", 0)->check(CLI::PositiveNumber);
  std::string nameTree{"Tree"};
  app.add_option("-t,--tree", nameTree, "Name of the TTree", "Tree");
  std::pair<double, double> SignalWindow;
  app.add_option("-s,--signal", SignalWindow, "Signal window")->required()->type_size(2);
  std::pair<double, double> NoiseWindow;
  app.add_option("-n,--noise", NoiseWindow, "Noise window")->required()->type_size(2);

  int NumberChambers{0};
  app.add_option("-c,--chambers", NumberChambers, "Number of chamber(s)")->check(CLI::PositiveNumber)->required();
  std::vector<int> distribution;
  app.add_option("-d,--distribution", distribution, "Channel is in wich chamber start at 0 and -1 if not connected")->required();
  std::vector<std::string> polarity;
  app.add_option("-p,--polarity", polarity, "Polarity of the signal Positive,+,Negative,-")->required();
  try
  {
    app.parse(argc, argv);
  }
  catch(const CLI::ParseError& e)
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

  Event* event{nullptr};
  if(Run->SetBranchAddress("Events", &event))
  {
    std::cout << "Error while SetBranchAddress !!!" << std::endl;
    std::exit(-5);
  }

  Long64_t NEntries = Run->GetEntries();
  if(NbrEvents > NEntries ) NbrEvents = NEntries;



  /*TH1D     sigmas_event("Ration_event", "Ration_event", 100, 0, 5);
  TH1D     sigmas_noise("Ration_event", "Ration_event", 100, 0, 5);
  double   scalefactor = 1.0;
  Channels channels;

  channels.activateChannel(9, "N");
  channels.activateChannel(10, "N");
  channels.activateChannel(11, "N");
  channels.activateChannel(12, "N");
  channels.activateChannel(13, "N");
  channels.activateChannel(14, "N");
  channels.activateChannel(15, "N");
  channels.activateChannel(16, "N");*/
  //channels.activateChannel(10, "N");
  //channels.activateChannel(11, "N");
  //channels.activateChannel(12, "N");
  //channels.activateChannel(13, "N");
  //  channels.activateChannel(14, "N");



 // channels.print();

 /* int    efficiency{0};
  bool   good = false;
  bool   hasseensomething{false};*/


  //TCanvas can;
  // std::vector<TH1D> Verif;
  for(Long64_t evt = 0; evt < NbrEvents; ++evt)
  {
    event->clear();
    Run->GetEntry(evt);
    for(unsigned int ch = 0; ch != event->Channels.size(); ++ch)
    {
    /*  if(channels.DontAnalyseIt(ch)) continue;  // Data for channel X is in file but i dont give a *** to
      // analyse it !

      TH1D                                                            waveform = CreateAndFillWaveform(evt, event->Channels[ch], "Waveform", "Waveform");
      std::pair<std::pair<double, double>, std::pair<double, double>> meanstd  = MeanSTD(event->Channels[ch], SignalWindow, NoiseWindow);

      std::cout<<meanstd.first.first<<" "<<meanstd.first.second<<" "<<meanstd.second.first<<" "<<meanstd.second.second<<std::endl;

      // std::cout<<"Event "<<evt<<" Channel "<<ch<<"/n";
      // std::cout<<" Mean : "<<meanstd.first<<" STD :
      // "<<meanstd.second<<std::endl;
      // selected with be updated each time we make some selection... For now
      // it's the same as Waveform one but in Red !!!
      // TH1D selected = CreateSelectionPlot(waveform);

      ///////////***********************************************************************
      ///////////***********************************************************************
      ///////////***********************************************************************
      ///////////***********************************************************************
      ///////////***********************************************************************
      ///////////***********************************************************************
      can.Clear();
      waveform.GetXaxis()->SetRangeUser(0, 1024);
      //waveform.Scale(1.0 / 4096);
      //waveform.Draw("HIST");
      //can.SaveAs(("ALL/ALL" + std::to_string(evt) + "_Channel" + std::to_string(ch) + ".pdf").c_str());


      if((meanstd.second.first - meanstd.first.first)*channels.getPolarity(ch)- (3 * (meanstd.first.second)) >0 ) hasseensomething = true;
      else hasseensomething = false;

      if(hasseensomething == true)
      {
        good = true;
        //hasseensomething=true;
        can.Clear();
        waveform.GetXaxis()->SetRangeUser(0, 1024);
        //waveform.Scale(1.0 / 4096);
        waveform.Draw("HIST");
        // if(channels.ShouldBePositive(ch)) waveform.Fit(f1);
        // else waveform.Fit(f1);
        can.SaveAs(("GOOD/GOOD" + std::to_string(evt) + "_Channel" + std::to_string(ch) + ".pdf").c_str());
        std::cout << "********************************************************************************************" << std::endl;
        std::cout << "GOODGOODGOODGOODGOODGOODGOODGOODGOODGOODGOODGOODGOODGOODGOODGOODGOODGOODGOODGOODGOODGOODGOODGOOD" << std::endl;
        std::cout << meanstd.second.first << "  " << meanstd.first.first << "   " << std::fabs(meanstd.second.first - meanstd.first.first) << "    " << 0.5 * meanstd.first.second << std::endl;
        std::cout << "GOODGOODGOODGOODGOODGOODGOODGOODGOODGOODGOODGOODGOODGOODGOODGOODGOODGOODGOODGOODGOODGOODGOODGOOD" << std::endl;
        std::cout << "********************************************************************************************" << std::endl;
      }
      else
      {
        can.Clear();
        waveform.GetXaxis()->SetRangeUser(0, 1024);
        // waveform.Scale(1.0 / 4096);
        waveform.Draw("HIST");
        // if(channels.ShouldBePositive(ch)) waveform.Fit(f1);
        // else waveform.Fit(f1);
        can.SaveAs(("BAD/BAD" + std::to_string(evt) + "_Channel" + std::to_string(ch) + ".pdf").c_str());
      }
    }
    if(good == true)
    {
      //hasseensomething=true;
      efficiency++;
      good = false;*/
    }
  }
 /* std::cout<<efficiency<<std::endl;
  std::cout << "Chamber efficiency " << efficiency * 100.00 / (NbrEvents * scalefactor) << std::endl;*/
  if(event != nullptr) delete event;
  if(Run != nullptr) delete Run;
  if(fileIn.IsOpen()) fileIn.Close();
}
