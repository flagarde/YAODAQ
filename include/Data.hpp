#ifndef DATA_H
#define DATA_H

#include "CAENDigitizer.h"
#include "CAENDigitizerType.h"
#include "WDplot.hpp"
#define MAX_GW  1000        /* max. number of generic write commads */
#define MAX_CH  64          /* max. number of channels */
#define MAX_SET 16           /* max. number of independent settings */
#define MAX_GROUPS  8          /* max. number of groups */
typedef enum {
	OFF_BINARY=	0x00000001,			// Bit 0: 1 = BINARY, 0 =ASCII
	OFF_HEADER= 0x00000002,			// Bit 1: 1 = include header, 0 = just samples data
} OUTFILE_FLAGS;

class DAC_Calibration_data
{
public:
	float cal[MAX_SET];
	float offset[MAX_SET];
};


class WaveDumpConfig_t
{
public :
    int LinkType;
    int LinkNum;
    int ConetNode;
    uint32_t BaseAddress;
    int Nch;
    int Nbit;
    float Ts;
    int NumEvents;
    uint32_t RecordLength;
    int PostTrigger;
    int InterruptNumEvents;
    int TestPattern;
    CAEN_DGTZ_EnaDis_t DesMode;
    CAEN_DGTZ_IOLevel_t FPIOtype;
    CAEN_DGTZ_TriggerMode_t ExtTriggerMode;
    uint16_t EnableMask;
    char GnuPlotPath[1000];
    CAEN_DGTZ_TriggerMode_t ChannelTriggerMode[MAX_SET];
	CAEN_DGTZ_PulsePolarity_t PulsePolarity[MAX_SET];
    uint32_t DCoffset[MAX_SET];
    int32_t  DCoffsetGrpCh[MAX_SET][MAX_SET];
    uint32_t Threshold[MAX_SET];
	int Version_used[MAX_SET];
	uint8_t GroupTrgEnableMask[MAX_SET];
    uint32_t MaxGroupNumber;
	
	uint32_t FTDCoffset[MAX_SET];
	uint32_t FTThreshold[MAX_SET];
	CAEN_DGTZ_TriggerMode_t	FastTriggerMode;
	uint32_t	 FastTriggerEnabled;
    int GWn;
    uint32_t GWaddr[MAX_GW];
    uint32_t GWdata[MAX_GW];
	uint32_t GWmask[MAX_GW];
	OUTFILE_FLAGS OutFileFlags;
	uint16_t DecimationFactor;
    int useCorrections;
    int UseManualTables;
    char TablesFilenames[MAX_X742_GROUP_SIZE][1000];
    CAEN_DGTZ_DRS4Frequency_t DRS4Frequency;
    int StartupCalibration;
	DAC_Calibration_data DAC_Calib;
};


class WaveDumpRun_t
{
public:
    int Quit;
    int AcqRun;
    int PlotType;
    int ContinuousTrigger;
    int ContinuousWrite;
    int SingleWrite;
    int ContinuousPlot;
    int SinglePlot;
    int SetPlotOptions;
    int GroupPlotIndex;
	int GroupPlotSwitch;
    int ChannelPlotMask;
    int RunHisto;
    uint32_t *Histogram[MAX_CH];
    FILE *fout[MAX_CH];
};

class Data
{
public:
  Data()
	{
			memset(&WDrun, 0, sizeof(WDrun));
			memset(&WDcfg, 0, sizeof(WDcfg));
	for (std::size_t i = 0; i < MAX_SET; i++) {
		WDcfg.DAC_Calib.cal[i] = 1;
		WDcfg.DAC_Calib.offset[i] = 0;
	}
	}
  ~Data()
  {
	if(buffer!=nullptr) delete buffer;
	if(EventPtr!=nullptr) delete buffer;
	if(PlotVar!=nullptr) ClosePlotter();
	if(Event8!=nullptr) delete Event8;
	if(Event16!=nullptr) delete Event16;
	if(Event742!=nullptr) delete Event742;
  }
  CAEN_DGTZ_UINT16_EVENT_t  *Event16{nullptr};
  CAEN_DGTZ_UINT8_EVENT_t   *Event8{nullptr};
  char *buffer{nullptr};
  char *EventPtr{nullptr};
  CAEN_DGTZ_BoardInfo_t       BoardInfo;
  CAEN_DGTZ_EventInfo_t       EventInfo;
  CAEN_DGTZ_X742_EVENT_t       *Event742{nullptr};  /* custom event struct with 8 bit data (only for 8 bit digitizers) */
  WDPlot_t                    *PlotVar{nullptr};
  WaveDumpConfig_t   WDcfg;
  WaveDumpRun_t      WDrun;
};
#endif
