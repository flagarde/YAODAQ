#ifndef CONFIG_H
#define CONFIG_H
#include "CAENDigitizerType.h"


typedef enum {
  OFF_BINARY = 0x00000001, // Bit 0: 1 = BINARY, 0 =ASCII
  OFF_HEADER = 0x00000002, // Bit 1: 1 = include header, 0 = just samples data
} OUTFILE_FLAGS;

#define MAX_GW 1000  /* max. number of generic write commads */
#define MAX_CH 64    /* max. number of channels */
#define MAX_SET 16   /* max. number of independent settings */
#define MAX_GROUPS 8 /* max. number of groups */

class DAC_Calibration_data {
public:
  float cal[MAX_SET];
  float offset[MAX_SET];
};


class WaveDumpConfig_t {
public:
  WaveDumpConfig_t() {
      for (std::size_t i = 0; i < MAX_SET; i++) {
      DAC_Calib.cal[i] = 1;
      DAC_Calib.offset[i] = 0;
    }
    for (std::size_t i = 0; i < MAX_SET; i++) {
      PulsePolarity[i] = CAEN_DGTZ_PulsePolarityPositive;
      Version_used[i] = 0;
      DCoffset[i] = 0;
      Threshold[i] = 0;
      ChannelTriggerMode[i] = CAEN_DGTZ_TRGMODE_DISABLED;
      GroupTrgEnableMask[i] = 0;
      for (std::size_t j = 0; j < MAX_SET; j++)
        DCoffsetGrpCh[i][j] = -1;
      FTThreshold[i] = 0;
      FTDCoffset[i] = 0;
    }
    for (std::size_t i = 0; i < MAX_X742_GROUP_SIZE; i++)
      sprintf(TablesFilenames[i], "Tables_gr%d", i);
  }
  int LinkType;
  int LinkNum;
  int ConetNode;
  uint32_t BaseAddress;
  int Nch;
  int Nbit;
  float Ts;
  int NumEvents{1023};
  uint32_t RecordLength{1024 * 16};
  int PostTrigger{50};
  int InterruptNumEvents{0};
  int TestPattern{0};
  CAEN_DGTZ_EnaDis_t DesMode{CAEN_DGTZ_EnaDis_t(0)};
  CAEN_DGTZ_IOLevel_t FPIOtype{CAEN_DGTZ_IOLevel_t(0)};
  CAEN_DGTZ_TriggerMode_t ExtTriggerMode{CAEN_DGTZ_TRGMODE_ACQ_ONLY};
  uint16_t EnableMask{0xFFFF};
  CAEN_DGTZ_TriggerMode_t ChannelTriggerMode[MAX_SET];
  CAEN_DGTZ_PulsePolarity_t PulsePolarity[MAX_SET];
  uint32_t DCoffset[MAX_SET];
  int32_t DCoffsetGrpCh[MAX_SET][MAX_SET];
  uint32_t Threshold[MAX_SET];
  int Version_used[MAX_SET];
  uint8_t GroupTrgEnableMask[MAX_SET];
  uint32_t MaxGroupNumber;

  uint32_t FTDCoffset[MAX_SET];
  uint32_t FTThreshold[MAX_SET];
  CAEN_DGTZ_TriggerMode_t FastTriggerMode{CAEN_DGTZ_TriggerMode_t(0)};
  uint32_t FastTriggerEnabled{0};
  int GWn{0};
  uint32_t GWaddr[MAX_GW];
  uint32_t GWdata[MAX_GW];
  uint32_t GWmask[MAX_GW];
  OUTFILE_FLAGS OutFileFlags;
  uint16_t DecimationFactor{1};
  int useCorrections{-1};
  int UseManualTables{-1};
  char TablesFilenames[MAX_X742_GROUP_SIZE][1000];
  CAEN_DGTZ_DRS4Frequency_t DRS4Frequency{CAEN_DGTZ_DRS4_5GHz};
  int StartupCalibration{1};
  DAC_Calibration_data DAC_Calib;
};
#endif
