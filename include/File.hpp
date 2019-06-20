#include "TFile.h"
#include "TTree.h"
#include <string>
#include <iostream>
#include <vector>
#include <bitset>

class File
{
public:
    void setName(const std::string& name)
    {
        m_name=name;
    }
    void createFile()
    {
        m_file = new TFile(m_name.c_str(),"RECREATE");
    }
    ~File()
    {
        m_file->Close();
        delete m_file;
    }
private:
    TFile* m_file{nullptr};
    std::string m_name{""};
    unsigned int nbrChannels{0};
};

class Tree : public TTree 
{
public:
    Tree(const std::string& name,const std::string& title):m_tree(new TTree(name.c_str(),title.c_str())){}
    void buildTree(unsigned int& nbrChambers)
    {
        m_NbrChannels=nbrChambers;
        m_data.reserve(m_NbrChannels);
        m_lastTrigTime.reserve(m_NbrChannels);
        m_nRollover.reserve(m_NbrChannels);
        std::stringstream ss;
        for(int i=0; i<m_NbrChannels; i++)
        {
            ss<<i;
            std::string branch="ch"+ss.str();
            ss.str("");
            if(mask[i]==1)
            {
                m_tree->Branch(branch.c_str(), &m_data[i]);
            }
            //time is the unix time that the event occured
            m_tree->Branch("Time", &m_eventTime, "Time/D");
            //Record length
            m_tree->Branch("Record_Length", &m_recordLength, "Record_Length/D");
            //Board ID
            m_tree->Branch("Board_ID", &m_boardID, "Board_ID/D");
            //Channel
            m_tree->Branch("Channel", &m_channel, "Channel/D");
            //Event Number
            m_tree->Branch("EventNbr", &m_eventNbr, "EventNbr/D");
            //Pattern
            m_tree->Branch("Pattern", &m_pattern, "Pattern/D");
            //DC offset
            m_tree->Branch("DC_Offset", &m_DCOffset, "DC_Offset/D");
            //Start Index Cell
            m_tree->Branch("Start_Index_Cell", &m_DCOffset, "Start_Index_Cell/D");
        }
    }
    
    
    void addEvent(CAEN_DGTZ_EventInfo_t *EventInfo, CAEN_DGTZ_UINT8_EVENT_t *Event8,WaveDumpConfig_t *WDcfg)
    {
        if(m_tree==nullptr)return;  
        for(unsigned int ch=0; ch!=m_data.size(); ++ch)
        {
            if(mask[ch]==0)continue;
            int Size=Event8->ChSize[ch];
            if(Size<=0)continue;
            else m_data[ch].reserve(Size);
            for(int i=0; i<Size; i++)
            {
                
                m_data[ch].push_back(Event8->DataChannel[ch][i]);
            }   
            if(EventInfo->TriggerTimeTag<m_lastTrigTime[ch])
            {
                m_nRollover[ch]++;
                m_lastTrigTime[ch]=0;
           
            }
            else m_lastTrigTime[ch]=EventInfo->TriggerTimeTag;
            m_eventTime = (double)EventInfo->TriggerTimeTag*8e-9+m_RunStartTime+m_nRollover[ch]*m_rolloverAdd; 
            m_recordLength=Size;
            m_boardID=EventInfo->BoardId;
            m_channel=ch;
            m_eventNbr=EventInfo->EventCounter;
            m_pattern=EventInfo->Pattern & 0xFFFF;
            m_DCOffset=WDcfg->DCoffset[ch] & 0xFFFF;
            m_tree->Fill();
        }
        clearData();
    }
    
    
    
    void addEvent(CAEN_DGTZ_EventInfo_t *EventInfo, CAEN_DGTZ_UINT16_EVENT_t *Event16,WaveDumpConfig_t *WDcfg)
    {
        if(m_tree==nullptr)return;  
        for(unsigned int ch=0; ch!=m_data.size(); ++ch)
        {
            if(mask[ch]==0)continue;
            int Size=Event16->ChSize[ch];
            if(Size<=0)continue;
            else m_data[ch].reserve(Size);
            for(int i=0; i<Size; i++)
            {
                
                m_data[ch].push_back(Event16->DataChannel[ch][i]);
            }   
            if(EventInfo->TriggerTimeTag<m_lastTrigTime[ch])
            {
                m_nRollover[ch]++;
                m_lastTrigTime[ch]=0;
           
            }
            else m_lastTrigTime[ch]=EventInfo->TriggerTimeTag;
            m_eventTime = (double)EventInfo->TriggerTimeTag*8e-9+m_RunStartTime+m_nRollover[ch]*m_rolloverAdd; 
            m_recordLength=Size;
            m_boardID=EventInfo->BoardId;
            m_channel=ch;
            m_eventNbr=EventInfo->EventCounter;
            m_pattern=EventInfo->Pattern & 0xFFFF;
            m_DCOffset=WDcfg->DCoffset[ch] & 0xFFFF;
            m_tree->Fill();
        }
        clearData();
    }
    
    void addEvent(CAEN_DGTZ_EventInfo_t *EventInfo, CAEN_DGTZ_X742_EVENT_t *Event,WaveDumpConfig_t *WDcfg)
    {
        char flag = 0; 
        for (unsigned int gr=0;gr<(WDcfg->Nch/8);gr++) 
        {   
            if (Event->GrPresent[gr])
            {
                for(unsigned int ch=0; ch<9; ch++) 
                {
                    int Size = Event->DataGroup[gr].ChSize[ch];
                    if (Size <= 0) continue;
                    // Write the Channel Header
                    if(Event->DataGroup[gr].TriggerTimeTag<m_lastTrigTime[ch])
                    {
                        m_nRollover[ch]++;
                        m_lastTrigTime[ch]=0;
                    }
                    else m_lastTrigTime[ch]=Event->DataGroup[gr].TriggerTimeTag;
                    m_eventTime = (double)Event->DataGroup[gr].TriggerTimeTag*8e-9+m_RunStartTime+m_nRollover[ch]*m_rolloverAdd; 
                    m_recordLength=Size;
                    m_boardID=EventInfo->BoardId;
                    m_channel=(gr*8)+ ch;
                    m_eventNbr=EventInfo->EventCounter;
                    m_pattern=EventInfo->Pattern & 0xFFFF;
                    m_DCOffset=WDcfg->DCoffset[ch] & 0xFFFF;
                    m_startIndexCell=Event->DataGroup[gr].StartIndexCell;
                    flag = 0;
                    for(unsigned j=0; j<Size; j++) 
                    {
                        m_data[(gr*8)+ ch].push_back(Event->DataGroup[gr].DataChannel[ch][j]);
                    }
                    m_tree->Fill();
                }
            }
        }
        clearData();
    }
    
    
    void setRunStartTime(const double& rstart)
    {
        m_RunStartTime=rstart;
    }
    ~Tree()
    {
        delete m_tree;
    }
private:
    const double m_rolloverAdd={8e-9*2147483647};
    double m_RunStartTime{0};
    double m_eventTime{0};
    double m_recordLength{0};
    double m_boardID{0};
    double m_eventNbr{0};
    double m_channel{0};
    double m_pattern{0};
    double m_DCOffset{0};
    double m_startIndexCell{0};
    std::vector<uint32_t> m_lastTrigTime;
    std::vector<int> m_nRollover;
    void clearData()
    {
        for(unsigned int ch=0; ch!=m_data.size(); ++ch) m_data[ch].clear();
    }
    TTree* m_tree{nullptr};
    unsigned int m_NbrChannels{0};
    std::vector<std::vector<double>> m_data;
    std::bitset<64> mask;
};
