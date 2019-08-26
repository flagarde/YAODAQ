#include <vector>
#include <bitset>
#include <CAENDigitizer.h>
#include <CAENDigitizerType.h>
#include "util.h"
#include "WaveDump.h"
#include "TFile.h"
#include "TTree.h"

//#include "CAENdaq.h"


#ifndef FILEMAN_h
#define FILEMAN_h


const double rolloverAdd=8e-9*2147483647;


class fileManager
{
    public:
    fileManager()
    {
        initialized=false;
    }
    fileManager(std::string filename, uint16_t EnableMask, int nbrChannels, double xinc)
    {
        setNbrChannels(nbrChannels);
        setTick(xinc);
        init(filename, EnableMask);
    }
    ~fileManager(){}
    
    void init(std::string filename, uint16_t EnableMask);
    void OpenFile();
    void addEvent(CAEN_DGTZ_EventInfo_t* EventInfo, CAEN_DGTZ_UINT16_EVENT_t* Event16);
    void addEvent(CAEN_DGTZ_EventInfo_t* EventInfo, CAEN_DGTZ_UINT8_EVENT_t* Event8);
    void CloseFile();
    void setVerbose(bool v)
    {
        verbose=v;
    }
    bool isInit()
    {
        return initialized;
    }
    void setRunStartTime(double rstart)
    {
        RunStartTime=rstart;
        for(int i=0; i<8; i++)
        {
            lastTrigTime[i]=0;
            nRollover[i]=0;
        }
    }
    void setNbrChannels(int i)
    {
        nbrChannels=i;
        lastTrigTime.resize(i);
        nRollover.resize(i);
        data.resize(i);
    }
    void setTick(double i)
    {
        xinc=i;
    }
    private:
    unsigned int nbrChannels{0};
    bool verbose{false};
    bool initialized{false};
    std::string fname{""};
    TFile *f{nullptr};
    TTree *t{nullptr};
    std::vector<std::vector<double>>data;
    std::bitset<MAX_CH> mask;
    double RunStartTime{0};
    double eventTime{0};
    bool isOpen{false};
    std::vector<uint32_t> lastTrigTime;
    std::vector<int> nRollover;
    double xinc{0};
    std::string finalFilename{""};
};
#endif
