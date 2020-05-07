#pragma once

#include "Channel.hpp"
#include "Data.hpp"
#include "Event.hpp"
#include "TFile.h"
#include "TTree.h"
#include "util.h"

#include <CAENDigitizerType.h>
#include <bitset>
#include <iostream>
#include <vector>

const double rolloverAdd = 8e-9 * 2147483647;

class FileManager
{
public:
  FileManager(Data& data): dat(data) { initialized = false; }
  void AddEvents();
  FileManager(Data& data, std::string filename, uint16_t EnableMask, int nbrChannels, double xinc): dat(data) { Init(filename, EnableMask, nbrChannels, xinc); }
  ~FileManager()
  {
    delete f;
    delete t;
    delete event;
  }

  void Init(const std::string& filename, uint16_t EnableMask, int nbrChannels, double xinc);
  void OpenFile();
  void CloseFile();
  void setVerbose(bool v) { verbose = v; }
  bool isInit() { return initialized; }
  void setRunStartTime(double rstart)
  {
    RunStartTime = rstart;
    for(int i = 0; i < 8; i++)
    {
      lastTrigTime[i] = 0;
      nRollover[i]    = 0;
    }
  }
  void setNbrChannels(int i)
  {
    nbrChannels = i;
    lastTrigTime.resize(i);
    nRollover.resize(i);
    // Channels.resize(i);
  }
  void setTick(double i) { xinc = i; }
  void addEvent()
  {
    if(dat.Event16 != nullptr) addEvent16();
    else if(dat.Event8 != nullptr)
      addEvent8();
    else if(dat.Event742 != nullptr)
      addEvent742();
  }
  std::string SHA512();

private:
  void addEvent16();

  void                  addEvent8();
  void                  addEvent742();
  unsigned int          nbrChannels{0};
  bool                  verbose{false};
  bool                  initialized{false};
  std::string           fname{""};
  std::string           path{"/mnt/NAS/RawData/"};
  TFile*                f{nullptr};
  TTree*                t{nullptr};
  std::bitset<64>       mask;
  double                RunStartTime{0};
  double                eventTime{0};
  bool                  isOpen{false};
  std::vector<uint32_t> lastTrigTime;
  std::vector<int>      nRollover;
  double                xinc{0};
  std::string           finalFilename{""};
  std::string           m_SHA512{""};
  Event*                event{nullptr};
  Data&                 dat;
};
