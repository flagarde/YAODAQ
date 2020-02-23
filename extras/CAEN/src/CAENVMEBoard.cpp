#include "CAENVMEBoard.hpp"

#include "CAENVMEException.hpp"
#include "CAENVMElib.h"

namespace CAEN
{
void Pulser::setIOSourcesStart(const unsigned char& source)
{
  m_IOSourcesStart = returnIOSources(source);
}

void Pulser::setIOSourcesStop(const unsigned char& source)
{
  m_IOSourcesStop = returnIOSources(source);
}

void Pulser::setTimeUnits(const unsigned char& time)
{
  m_TimeUnits = returnTimeUnits(time);
}

void Pulser::setIOSourcesStart(const std::string& source)
{
  m_IOSourcesStart = source;
}

void Pulser::setIOSourcesStop(const std::string& source)
{
  m_IOSourcesStop = source;
}

std::string Pulser::returnIOSources(const unsigned char& source)
{
  if(source == cvManualSW) return "ManualSW";
  else if(source == cvInputSrc0)
    return "InputSrc0";
  else if(source == cvInputSrc1)
    return "InputSrc1";
  else if(source == cvCoincidence)
    return "Coincidence";
  else if(source == cvVMESignals)
    return "VMESignals";
  else if(source == cvMiscSignals)
    return "MiscSignals";
  else
    throw - 1;
}

std::string Pulser::returnTimeUnits(const unsigned char& source)
{
  if(source == cvUnit25ns) return "25ns";
  else if(source == cvUnit1600ns)
    return "1600ns";
  else if(source == cvUnit410us)
    return "410us";
  else if(source == cvUnit104ms)
    return "104ms";
  else
    throw - 1;
}

unsigned char Pulser::returnIOSources(const std::string& source)
{
  if(source == "ManualSW") return cvManualSW;
  else if(source == "InputSrc0")
    return cvInputSrc0;
  else if(source == "InputSrc1")
    return cvInputSrc1;
  else if(source == "Coincidence")
    return cvCoincidence;
  else if(source == "VMESignals")
    return cvVMESignals;
  else if(source == "MiscSignals")
    return cvMiscSignals;
  else
    throw - 1;
}

unsigned char Pulser::returnTimeUnits(const std::string& source)
{
  if(source == "25ns") return cvUnit25ns;
  else if(source == "1600ns")
    return cvUnit1600ns;
  else if(source == "410us")
    return cvUnit410us;
  else if(source == "104ms")
    return cvUnit104ms;
  else
    throw - 1;
}

void Pulser::setTimeUnits(const std::string& time)
{
  m_TimeUnits = time;
}

void Pulser::setPeriod(const unsigned int& period)
{
  m_Period = period;
}

void Pulser::setWidth(const unsigned int& width)
{
  m_Width = width;
}

void Pulser::setPulseNbr(const unsigned int& pulsenbr)
{
  m_PulseNbr = pulsenbr;
}

std::string Pulser::getIOSourcesStart()
{
  return m_IOSourcesStart;
}

std::string Pulser::getIOSourcesStop()
{
  return m_IOSourcesStop;
}

std::string Pulser::getTimeUnits()
{
  return m_TimeUnits;
}

unsigned char Pulser::returnIOSourcesStart()
{
  return returnIOSources(m_IOSourcesStart);
}

unsigned char Pulser::returnIOSourcesStop()
{
  return returnIOSources(m_IOSourcesStop);
}

unsigned char Pulser::returnTimeUnits()
{
  return returnTimeUnits(m_TimeUnits);
}

unsigned char Pulser::getPeriod()
{
  return m_Period;
}

unsigned char Pulser::getWidth()
{
  return m_Width;
}

unsigned char Pulser::getPulseNbr()
{
  return m_PulseNbr;
}

CAENVMEBoard::CAENVMEBoard(const std::string& name): Board(name, "CAENVMEBoard")
{
}

void CAENVMEBoard::StartPulserA()
{
  CAENVMEException(CAENVME_StartPulser(m_Handle, cvPulserA));
}

void CAENVMEBoard::StartPulserB()
{
  CAENVMEException(CAENVME_StartPulser(m_Handle, cvPulserB));
}

void CAENVMEBoard::StopPulserA()
{
  CAENVMEException(CAENVME_StopPulser(m_Handle, cvPulserA));
}

void CAENVMEBoard::StopPulserB()
{
  CAENVMEException(CAENVME_StopPulser(m_Handle, cvPulserB));
}

void CAENVMEBoard::SetPulserAConf(Pulser& p)
{
  CAENVMEException(
      CAENVME_SetPulserConf(m_Handle, cvPulserA, p.getPeriod(), p.getWidth(),
                            CVTimeUnits(p.returnTimeUnits()), p.getPulseNbr(),
                            CVIOSources(p.returnIOSourcesStart()),
                            CVIOSources(p.returnIOSourcesStop())));
}

void CAENVMEBoard::SetPulserBConf(Pulser& p)
{
  CAENVMEException(
      CAENVME_SetPulserConf(m_Handle, cvPulserB, p.getPeriod(), p.getWidth(),
                            CVTimeUnits(p.returnTimeUnits()), p.getPulseNbr(),
                            CVIOSources(p.returnIOSourcesStart()),
                            CVIOSources(p.returnIOSourcesStop())));
}

Pulser CAENVMEBoard::GetPulserAConf()
{
  unsigned char period{0};
  unsigned char width{0};
  unsigned char pulseno{0};
  CVTimeUnits   units;
  CVIOSources   start;
  CVIOSources   stop;
  Pulser        p;
  CAENVMEException(CAENVME_GetPulserConf(m_Handle, cvPulserA, &period, &width,
                                     &units, &pulseno, &start, &stop));
  p.setPeriod(period);
  p.setWidth(width);
  p.setPulseNbr(pulseno);
  p.setTimeUnits(units);
  p.setIOSourcesStart(start);
  p.setIOSourcesStop(stop);
  return p;
}

Pulser CAENVMEBoard::GetPulserBConf()
{
  unsigned char period{0};
  unsigned char width{0};
  unsigned char pulseno{0};
  CVTimeUnits   units;
  CVIOSources   start;
  CVIOSources   stop;
  Pulser        p;
  CAENVMEException(CAENVME_GetPulserConf(m_Handle, cvPulserB, &period, &width,
                                     &units, &pulseno, &start, &stop));
  p.setPeriod(period);
  p.setWidth(width);
  p.setPulseNbr(pulseno);
  p.setTimeUnits(units);
  p.setIOSourcesStart(start);
  p.setIOSourcesStop(stop);
  return p;
}

/*
std::string CAENVMEBoard::SWRelease()
{
    char swrelease[10];
    CAENVMEException(CAENVME_SWRelease(swrelease));
    return swrelease;
}

std::string CAENVMEBoard::BoardFWRelease()
{
    char fwrelease[10];
    CAENVMEException(CAENVME_BoardFWRelease(m_Handle,fwrelease));
    return fwrelease;
}

std::string CAENVMEBoard::DriverRelease()
{
    char driver[10];
    CAENVMEException(CAENVME_DriverRelease(m_Handle,driver));
    return driver;
}

void CAENVMEBoard::DeviceReset()
{
#ifdef UNIX
    CAENVMEException(CAENVME_DeviceReset(m_Handle));
#else
    std::cout<<"DeviceReset not available on Windows"<<std::endl;
#endif
}*/

}  // namespace CAEN
