#pragma once

#include "Data.hpp"
#include "TBufferJSON.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TH1D.h"
#include "WebServer.hpp"

#include <iostream>

class Plotter
{
public:
  Plotter(Data& da, WebServer& ser);
  void PlotWaveform();
  void PlotHistograms();
  void SaveWaveForms();
  void SaveHistograms();
  void SaveFFT();
  void Plot();
  void Upload();

private:
  void SetLabelsWaveForms()
  {
    /*	for(std::size_t h=0;h!=histos.size();++h)
      {
if (BoardInfo.FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE)
histos[h].SetTitle("Waveform;ns;ADC counts"); else
histos[h].SetTitle("Waveform;us;ADC counts");
            }
histos[h].GetYaxis()->SetRange(0,(float)(1<<WDcfg.Nbit));*/
    /*  PlotVar->Xscale = WDcfg.Ts;
      PlotVar->Xautoscale = 1;
  } else if (WDrun.PlotType == PLOT_WAVEFORMS) {
      PlotVar->Xscale = WDcfg.Ts * WDcfg.DecimationFactor/1000;
      PlotVar->Xautoscale = 1;
  }*/
  }
  void SetLabelsFFT(const std::vector<TH1*>& histo)
  {
    /*	for(std::size_t h=0;h!=histos.size();++h)
      {
                    histos[h].SetTitle("FFT;MHz;dB");
            }*/

    /*  PlotVar->Yautoscale = 1;
      PlotVar->Ymin = -160;
      PlotVar->Ymax = 0;
      PlotVar->Xautoscale = 1;*/
  }
  void SetLabelsHisto()
  {
    /*for(std::size_t h=0;h!=histos.size();++h)
    {
            histos[h].SetTitle("Histogram;ADC channels;Counts");
    }*/
    /*    PlotVar->Xscale = 1.0;

        PlotVar->Yautoscale = 1;
        PlotVar->Xautoscale = 1;*/
  }
  TCanvas             can;
  std::vector<TH1D>   histos;
  std::vector<TGraph> graphs;
  std::vector<TH1D>   histos_histos;
  TH1D                RE;
  TH1D                IM;
  TH1D                MAG;
  TH1D                PH;

public:
  Data& dat;
  void  Init();
  void  swapContinuousPlotting()
  {
    if(m_ContinuousPlotting == true) m_ContinuousPlotting = false;
    else
      m_ContinuousPlotting = true;
    std::cout << "Continuous Plotting is set to : " << m_ContinuousPlotting << std::endl;
  }
  bool isContinuousPlotting() { return m_ContinuousPlotting; }

private:
  bool m_ContinuousPlotting{"False"};
  int  NbrThreadFFT{0};
  bool isInitialised{false};
  Plotter() = delete;
  double     tickSize{0};
  WebServer& server;
};
