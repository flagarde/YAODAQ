#include "Plotter.hpp"

#include "TROOT.h"

#include <iostream>
#include <thread>

void Plotter::Upload()
{
  /*if(dat.WDrun.isNewEvent==true)
  {*/
  std::thread t(&Plotter::PlotHistograms, this);
  std::thread t2(&Plotter::PlotWaveform, this);
  t.join();
  t2.join();
  dat.WDrun.isNewEvent = false;
  /*}*/
}

void Plotter::Plot()
{
  // PlotHistograms();
  // PlotWaveform();
  std::thread wave(&Plotter::SaveWaveForms, this);
  std::thread hist(&Plotter::SaveHistograms, this);
  wave.detach();
  hist.detach();
  // SaveWaveForms();
  // SaveFFT();
  // SaveHistograms();
}

void Plotter::Init()
{
  TH1::AddDirectory(kFALSE);
  if(isInitialised == false)
  {
    // Generate TH1 for waveforms;
    int   NbrBins = /*FindMaxBin();*/ dat.WDcfg.RecordLength;
    float maxValue{0};
    if(dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE)
      tickSize = dat.WDcfg.Ts;
    else
      tickSize = (dat.WDcfg.Ts * dat.WDcfg.DecimationFactor / 1000);
    maxValue = NbrBins;
    for(std::size_t ch = 0; ch != dat.WDcfg.Nch; ++ch)
    {
      static int  erasetrigger{0};
      std::string title = "";
      if((dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE) &&
         ((ch != 0) && (((ch + 1) % 9) == 0)))
      { title = "Trigger" + std::to_string((int)((ch) / 8) - 1); }
      else
      {
        title = "Channel" + std::to_string(erasetrigger);

        erasetrigger++;
      }
      // graphs.push_back(TGraph(NbrBins));
      // graphs[ch].SetNameTitle(title.c_str(), title.c_str());
      histos.push_back(
          TH1D((title).c_str(), (title).c_str(), NbrBins, 0, maxValue));
      // histos[ch].GetYaxis()->SetLimits(-1.0,1.0);
      histos[ch].GetYaxis()->SetRangeUser(-1.0, 1.0);

      // histos[ch].SetAxisRange(-1.0, 1.0,"Y");
      if(ch == dat.WDcfg.Nch - 1) erasetrigger = 0;
    }
    // Generate TH1 for histos;
    uint64_t Nbrbins = (uint64_t)(1 << dat.WDcfg.Nbit);
    for(std::size_t ch = 0; ch != dat.WDcfg.Nch; ++ch)
    {
      static int  erasetrigger{0};
      std::string title = "";
      if((dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE) &&
         ((ch != 0) && (((ch + 1) % 9) == 0)))
      { title = "HistoTrigger" + std::to_string((int)((ch) / 8) - 1); }
      else
      {
        title = "HistoChannel" + std::to_string(erasetrigger);

        erasetrigger++;
      }

      histos_histos.push_back(
          TH1D(title.c_str(), title.c_str(), Nbrbins, 0, Nbrbins));
      if(ch == dat.WDcfg.Nch - 1) erasetrigger = 0;
    }

    // Avoid creating each time a new TH1D pointor
    RE  = TH1D("", "", NbrBins, 0, maxValue);
    IM  = TH1D("", "", NbrBins, 0, maxValue);
    MAG = TH1D("", "", NbrBins, 0, maxValue);
    PH  = TH1D("", "", NbrBins, 0, maxValue);
    // Initialiase the FFT
    TH1* RE2      = histos[0].FFT(&RE, "RE R2C ES");
    TH1* IM2      = histos[0].FFT(&IM, "IM R2C ES");
    TH1* MAG2     = histos[0].FFT(&MAG, "MAG R2C ES");
    TH1* PH2      = histos[0].FFT(&PH, "PH R2C ES");
    isInitialised = true;
  }
}

Plotter::Plotter(Data& da, WebServer& ser): dat(da), server(ser)
{
  ROOT::EnableImplicitMT();
  if(dat.WDcfg.Nch != 0) Init();
}

void Plotter::PlotHistograms()
{
  /*
  for(unsigned int i=0;i!=histos_histos.size();++i) histos[i].Reset("ICESM");
    if (dat.WDcfg.Nbit == 8) {
      for (std::size_t ch = 0; ch != dat.WDcfg.Nch; ++ch) {
        int Size = dat.Event8->ChSize[ch];
        if (Size <= 0)
          continue;
        for (int j = 0; j < Size; j++) {
          histos_histos[ch].Fill(dat.Event8->DataChannel[ch][j]);
        }
      }
    } else if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE) {
      for (std::size_t gr = 0; gr < (dat.WDcfg.Nch / 8); gr++) {
        if (dat.Event742->GrPresent[gr]) {
          for (std::size_t ch = 0; ch < 9; ch++) {
            int Size = dat.Event742->DataGroup[gr].ChSize[ch];
            if (Size <= 0)
              continue;
            for (int j = 0; j < Size; j++) {
              histos_histos[gr * 9 +
  ch].Fill(dat.Event742->DataGroup[gr].DataChannel[ch][j]);
            }
          }
        }
      }
    } else {
      for (std::size_t ch = 0; ch != dat.WDcfg.Nch; ++ch) {
        int Size = dat.Event16->ChSize[ch];
        if (Size <= 0)
          continue;
        for (int j = 0; j < Size; j++) {
          histos_histos[ch].Fill(dat.Event16->DataChannel[ch][j]);
        }
      }
    }


  */

  /*

    for (std::size_t ch = 0; ch < dat.WDcfg.Nch; ch++) {
      uint64_t Nbrbins = (uint64_t)(1 << dat.WDcfg.Nbit);
      std::string title = "HistosChannel" + std::to_string(ch);
      histos_histos.push_back(
          TH1D(title.c_str(), title.c_str(), Nbrbins, 0, Nbrbins));
      int chmask = ((dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX740_FAMILY_CODE) ||
                    (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE))
                       ? (ch / 8)
                       : ch;
      if (!(dat.EventInfo.ChannelMask & (1 << chmask))) continue;
      if (dat.WDcfg.Nbit == 8) {
        for (std::size_t i = 0; i < (int)dat.Event8->ChSize[ch]; i++)
          histos_histos[ch].Fill(dat.Event8->DataChannel[ch][i]);
      } else {
        if (dat.BoardInfo.FamilyCode != CAEN_DGTZ_XX742_FAMILY_CODE) {
          for (std::size_t i = 0; i < (int)dat.Event16->ChSize[ch]; i++)
            histos_histos[ch].Fill(dat.Event16->DataChannel[ch][i]);
        } else {
          for (std::size_t i = 0; i < (int)dat.Event16->ChSize[ch]; i++)
            histos_histos[ch].Fill(dat.Event16->DataChannel[ch][i]);
        }
      }
    }*/
}

void Plotter::PlotWaveform()
{
  float ti = 1.0 / (1 << dat.WDcfg.Nbit);
  for(unsigned int i = 0; i != histos.size(); ++i) histos[i].Reset("ICESM");
  if(dat.WDcfg.Nbit == 8)
  {
    for(std::size_t ch = 0; ch != dat.WDcfg.Nch; ++ch)
    {
      int Size = dat.Event8->ChSize[ch];
      if(Size <= 0) continue;
      for(int j = 0; j < Size; j++)
      { histos[ch].Fill(j, dat.Event8->DataChannel[ch][j] * ti); }
    }
  }
  else if(dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE)
  {
    for(std::size_t gr = 0; gr < (dat.WDcfg.Nch / 8); gr++)
    {
      if(dat.Event742->GrPresent[gr])
      {
        for(std::size_t ch = 0; ch < 9; ch++)
        {
          int Size = dat.Event742->DataGroup[gr].ChSize[ch];
          if(Size <= 0) continue;
          for(int j = 0; j < Size; j++)
          {
            histos[gr * 9 + ch].Fill(
                j, dat.Event742->DataGroup[gr].DataChannel[ch][j] * ti);
          }
        }
      }
    }
  }
  else
  {
    for(std::size_t ch = 0; ch != dat.WDcfg.Nch; ++ch)
    {
      int Size = dat.Event16->ChSize[ch];
      if(Size <= 0) continue;
      for(int j = 0; j < Size; j++)
      { histos[ch].Fill(j, dat.Event16->DataChannel[ch][j] * ti); }
    }
  }

  // for(unsigned int i=0;i!=histos.size();++i) graphs[i].Reset("ICESM");
  /* if (dat.WDcfg.Nbit == 8) {
     for (std::size_t ch = 0; ch != dat.WDcfg.Nch; ++ch) {
       int Size = dat.Event8->ChSize[ch];
       if (Size <= 0)
         continue;
       for (int j = 0; j < Size; j++) {
         graphs[ch].SetPoint(j,j*tickSize, dat.Event8->DataChannel[ch][j]*ti);
       }
     }
   } else if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE) {
     for (std::size_t gr = 0; gr < (dat.WDcfg.Nch / 8); gr++) {
       if (dat.Event742->GrPresent[gr]) {
         for (std::size_t ch = 0; ch < 9; ch++) {
           int Size = dat.Event742->DataGroup[gr].ChSize[ch];
           if (Size <= 0)
             continue;
           for (int j = 0; j < Size; j++) {

               graphs[gr * 9 + ch].SetPoint(j,j*tickSize,
   dat.Event742->DataGroup[gr].DataChannel[ch][j]*ti); histos[gr * 9 +
   ch].Fill(j, dat.Event742->DataGroup[gr].DataChannel[ch][j]*ti);
           }
         }
       }
     }
   } else {
     for (std::size_t ch = 0; ch != dat.WDcfg.Nch; ++ch) {
       int Size = dat.Event16->ChSize[ch];
       if (Size <= 0)
         continue;
       for (int j = 0; j < Size; j++) {
           graphs[ch].SetPoint(j,j*tickSize,dat.Event16->DataChannel[ch][j]*ti);
         histos[ch].Fill(j, dat.Event16->DataChannel[ch][j]*ti);
       }
     }
   }

   */
}

void Plotter::SaveFFT()
{
  NbrThreadFFT++;
  std::cout << NbrThreadFFT << std::endl;
  // auto &echo_all = server.endpoint["^/Rack/?$"];
  TH1*    toto{nullptr};
  TString json;
  for(std::size_t i = 0; i != histos.size(); ++i)
  {
    toto = histos[i].FFT(&RE, "RE R2C ES");
    toto->SetTitle(
        ("FFT Real Part " + std::string(histos[i].GetName())).c_str());
    toto->SetName(("RE" + std::string(histos[i].GetName())).c_str());
    json = TBufferJSON::ToJSON(toto);
    /* for (auto&& client : server.getClients())
                              {

                                      client->send(json.Data());

                              }*/
    toto = histos[i].FFT(&IM, "IM R2C ES");
    toto->SetTitle(
        ("FFT Imaginary Part " + std::string(histos[i].GetName())).c_str());
    toto->SetName(("IM" + std::string(histos[i].GetName())).c_str());
    json = TBufferJSON::ToJSON(toto);
    /* for (auto&& client : server.getClients())
                             {

                                     client->send(json.Data());

                             }*/
    toto = histos[i].FFT(&MAG, "MAG R2C ES");
    toto->SetTitle(
        ("FFT Magnitude " + std::string(histos[i].GetName())).c_str());
    toto->SetName(("MAG" + std::string(histos[i].GetName())).c_str());
    json = TBufferJSON::ToJSON(toto);
    /*for (auto&& client : server.getClients())
                               {

                                       client->send(json.Data());

                               }*/
    toto = histos[i].FFT(&PH, "PH R2C ES");
    toto->SetTitle(("FFT Phase " + std::string(histos[i].GetName())).c_str());
    toto->SetName(("PH" + std::string(histos[i].GetName())).c_str());
    json = TBufferJSON::ToJSON(toto);
    /*for (auto&& client : server.getClients())
                              {

                                      client->send(json.Data());

                              }*/
  }
  NbrThreadFFT--;
}

void Plotter::SaveWaveForms()
{
  for(std::size_t i = 0; i != histos.size(); ++i)
  {
    TString json = TBufferJSON::ToJSON(&histos[i]);
    server.SendInfos("Plot", json.Data());
  }
  // auto &echo_all = server.endpoint["^/Rack/?$"];

  /* for (std::size_t i = 0; i != graphs.size(); ++i) {
   TString json = TBufferJSON::ToJSON(&graphs[i]);
  // auto &echo_all = server.endpoint["^/Rack/?$"];
 for (auto&& client : server.getClients())
                           {

           //std::cout<<json.Data()<<std::endl;
                                   client->send(json.Data());

                           }
 }*/

  // if(NbrThreadFFT==0) SaveFFT();
  // else std::cout<<"Not plotting FFT again !!!! TOO FAST !!!"<<std::endl;
}

void Plotter::SaveHistograms()
{ /*
                                           for (std::size_t i = 0; i !=
                          histos_histos.size(); ++i) { TString json =
                          TBufferJSON::ToJSON(&histos_histos[i]);
                                              // auto &echo_all =
                          server.endpoint["^/Rack/?$"]; for (auto&& client :
                          server.getClients())
                                                                       {

                                                                               client->send(json.Data());

                                                                       }
                                             }*/
}
