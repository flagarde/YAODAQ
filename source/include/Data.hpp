#pragma once

#include "Config.hpp"

#include <cstdio>

class WaveDumpRun_t {
public:
  bool ContinuousPlot{false};
  int RunHisto{1};
  bool isNewEvent{false};
};

class Data {
public:
  Data() {}
  ~Data() {
    if (Event8 != nullptr)
      delete Event8;
    if (Event16 != nullptr)
      delete Event16;
    if (Event742 != nullptr)
      delete Event742;
  }
  CAEN_DGTZ_UINT16_EVENT_t *Event16{nullptr};
  CAEN_DGTZ_UINT8_EVENT_t *Event8{nullptr};
  CAEN_DGTZ_BoardInfo_t BoardInfo;
  CAEN_DGTZ_EventInfo_t EventInfo;
  CAEN_DGTZ_X742_EVENT_t *Event742{
      nullptr}; /* custom event struct with 8 bit data (only for 8 bit
                   digitizers) */
  WaveDumpConfig_t WDcfg;
  bool isTimeOut{false};
  int NbrEvents{0};
  WaveDumpRun_t WDrun;
};
