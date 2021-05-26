#include "CAENCommException.hpp"
#include "CAENDPPException.hpp"
#include "CAENDigitizerException.hpp"
#include "CAENFlashException.hpp"
#include "CAENHVException.hpp"
#include "CAENVMEException.hpp"

#include "doctest/doctest.h"

#include <iostream>

using namespace CAEN;

void VMEErrors()
{
  std::cout << "VME Errors" << std::endl;
  for(unsigned int i = 0; i != 6; ++i)
  {
    try
    {
      CAENVMEException(-i);
    }
    catch(std::exception& evet)
    {
      std::cout << evet.what() << std::endl;
    }
  }
}

void CommErrors()
{
  std::cout << "CAENComm Errors" << std::endl;
  for(unsigned int i = 0; i != 14; ++i)
  {
    try
    {
      CAENCommException(-i);
    }
    catch(std::exception& evet)
    {
      std::cout << evet.what() << std::endl;
    }
  }
}

void HVErrors()
{
  std::cout << "CAENHV Errors" << std::endl;
  for(int i = 0; i != 38; ++i)
  {
    try
    {
      if(i <= 30) CAENHVException(+i);
      else
      {
        int toto = (0x1000 + i);
        CAENHVException(+toto);
      }
    }
    catch(std::exception& evet)
    {
      std::cout << evet.what() << std::endl;
    }
  }
}

void DigitizerErrors()
{
  std::cout << "CAENDigitizer Errors" << std::endl;
  for(unsigned int i = 0; i != 35; ++i)
  {
    try
    {
      CAENDigitizerException(-i);
    }
    catch(std::exception& evet)
    {
      std::cout << evet.what() << std::endl;
    }
  }
}

void DPPErrors()
{
  std::cout << "CAENDPP Errors" << std::endl;
  for(unsigned int i = 100; i != 145; ++i)
  {
    try
    {
      CAENDPPException(-i);
    }
    catch(std::exception& evet)
    {
      std::cout << evet.what() << std::endl;
    }
  }
}

void FlashErrors()
{
  std::cout << "CAENFlash Errors" << std::endl;
  for(unsigned int i = 0; i != 8; ++i)
  {
    try
    {
      CAENFlashException(-i);
    }
    catch(std::exception& evet)
    {
      std::cout << evet.what() << std::endl;
    }
  }
}

TEST_CASE("Test CAEN Errors")
{
  VMEErrors();
  CommErrors();
  HVErrors();
  DigitizerErrors();
  DPPErrors();
  FlashErrors();
}
