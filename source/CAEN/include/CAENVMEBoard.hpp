#ifndef CAENVMEBOARD_HPP
#define CAENVMEBOARD_HPP
#include "Board.hpp"

namespace CAEN
{
    
class Pulser
{
public:
  void setIOSourcesStart(const std::string& source);
  void setIOSourcesStop(const std::string& source);
  void setTimeUnits(const std::string& time);
  void setIOSourcesStart(const unsigned char& source);
  void setIOSourcesStop(const unsigned char& source);
  void setTimeUnits(const unsigned char& time);
  void setPeriod(const unsigned int& period);
  void setWidth(const unsigned int& width);
  void setPulseNbr(const unsigned int& pulsenbr);
  unsigned char getPeriod();
  unsigned char getWidth();
  unsigned char getPulseNbr();
  std::string getIOSourcesStart();
  std::string getIOSourcesStop();
  std::string getTimeUnits();
  unsigned char returnIOSourcesStart();
  unsigned char returnIOSourcesStop();
  unsigned char returnTimeUnits();
private:
  unsigned char returnIOSources(const std::string& source);
  unsigned char returnTimeUnits(const std::string& source);
  std::string returnIOSources(const unsigned char& source);
  std::string returnTimeUnits(const unsigned char& source);
  std::string m_IOSourcesStart{""};
  std::string m_IOSourcesStop{""};
  unsigned char m_Period{0};
  unsigned char m_Width{0};
  unsigned char m_PulseNbr{0};
  std::string m_TimeUnits{""};
};
    
class Scaler
{
public:
    
private:
    
};

class CAENVMEBoard : public Board
{
public:
  CAENVMEBoard(const std::string& name="Unknown");
  void SetPulserAConf(Pulser& pulser);
  void SetPulserBConf(Pulser& pulser);
  Pulser GetPulserAConf();
  Pulser GetPulserBConf();
  void StartPulserA();
  void StartPulserB();
  void StopPulserA();
  void StopPulserB();
  
  
  
  
  
  
  
  
   /* std::string SWRelease();
    std::string BoardFWRelease();
    std::string DriverRelease();
    void DeviceReset();*/
    
private:
    

};

}
#endif
