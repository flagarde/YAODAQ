#ifndef PLOTTER_H
#define PLOTTER_H
#include "Data.hpp"
#include "TBufferJSON.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "THStack.h"
#include "server_ws.hpp"
using WsServer = SimpleWeb::SocketServer<SimpleWeb::WS>;

class Plotter {
public:
  Plotter(Data &da, WsServer &ser);
  void PlotWaveform();
  void PlotHistograms();
  void SaveWaveForms();
  void SaveHistograms();
  void SaveFFT();
  void Update();
  void OneTimePlot() 
  {
    if (dat.WDrun.ChannelPlotMask == 0) std::cout << "No channel enabled for plotting" << std::endl;
    else
    {
	dat.WDrun.SinglePlot = 1;
	Update();
    }
  }
private:
  /*int FindMaxBin() {
    int extreme{0};
    if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE) {
      for (std::size_t gr = 0; gr < (dat.WDcfg.Nch / 8); gr++) {
        if (dat.Event742->GrPresent[gr]) {
          for (std::size_t ch = 0; ch < 9; ch++) {
            int Size = dat.Event742->DataGroup[gr].ChSize[ch];
            if (Size <= 0)
              continue;
            if (Size > extreme)
              extreme = Size;
          }
        }
      }
    } else {
      for (std::size_t ch = 0; ch < dat.WDcfg.Nch; ch++) {
        int Size = (dat.WDcfg.Nbit == 8) ? dat.Event8->ChSize[ch]
                                         : dat.Event16->ChSize[ch];
        if (Size <= 0)
          continue;
        if (Size > extreme)
          extreme = Size;
      }
    }
    if (extreme == 0)
      return 1;
    return extreme;
  }*/
  void SetLabelsWaveForms() {
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
  void SetLabelsFFT(const std::vector<TH1 *> &histo) {
    /*	for(std::size_t h=0;h!=histos.size();++h)
      {
                    histos[h].SetTitle("FFT;MHz;dB");
            }*/

    /*  PlotVar->Yautoscale = 1;
      PlotVar->Ymin = -160;
      PlotVar->Ymax = 0;
      PlotVar->Xautoscale = 1;*/
  }
  void SetLabelsHisto() {
    /*for(std::size_t h=0;h!=histos.size();++h)
    {
            histos[h].SetTitle("Histogram;ADC channels;Counts");
    }*/
    /*    PlotVar->Xscale = 1.0;

        PlotVar->Yautoscale = 1;
        PlotVar->Xautoscale = 1;*/
  }
  THStack hs;
  TCanvas can;
  std::vector<TH1D> histos;
  std::vector<TH1D> histos_histos;
  TH1D RE;
  TH1D IM;
  TH1D MAG;
  TH1D PH;

public:
  Data &dat;
  void Init();
private:
  int NbrThreadFFT{0};
  bool isInitialised{false};
  Plotter() = delete;
  
  WsServer &server;
};
#endif
