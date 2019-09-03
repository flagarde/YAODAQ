#include <vector>
#include <bitset>
#include <CAENDigitizer.h>
#include <CAENDigitizerType.h>
#include "util.h"
#include "TFile.h"
#include "TTree.h"
#include "Channel.hpp"
#include "Data.hpp"
#include <iostream>
//#include "CAENdaq.h"


#ifndef FILEMAN_h
#define FILEMAN_h


const double rolloverAdd=8e-9*2147483647;


class FileManager
{
    public:
    FileManager(Data& data):dat(data)
    {
        initialized=false;
    }
    FileManager(Data& data,std::string filename, uint16_t EnableMask, int nbrChannels, double xinc):dat(data)
    {
        setNbrChannels(nbrChannels);
        setTick(xinc);
        Init(filename, EnableMask);
    }
    ~FileManager(){}
    
    void Init(std::string filename, uint16_t EnableMask);
    void OpenFile();
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
        Channels.resize(i);
    }
    void setTick(double i)
    {
        xinc=i;
    }
    void addEvent()
    {
	if(dat.Event16!=nullptr)  addEvent16();
	else if(dat.Event8!=nullptr)  addEvent8();
	else if(dat.Event742!=nullptr)  addEvent742();


    }
    private:
    void addEvent16();
    void addEvent8();
    void addEvent742();
    unsigned int nbrChannels{0};
    bool verbose{false};
    bool initialized{false};
    std::string fname{""};
    std::string path{"."};
    TFile* f{nullptr};
    TTree* t{nullptr};
    std::bitset<64> mask;
    double RunStartTime{0};
    double eventTime{0};
    bool isOpen{false};
    std::vector<uint32_t> lastTrigTime;
    std::vector<int> nRollover;
    double xinc{0};
    std::string finalFilename{""};


    //Per Event
    
    double BoardID{0};
    double EventNumber{0};
    double Pattern{0};
    

    
    std::vector<Channel>Channels;
    

	Data& dat;










};
#endif
