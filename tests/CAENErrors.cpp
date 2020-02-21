#include "CAENCommError.hpp"
#include "CAENDPPError.hpp"
#include "CAENDigitizerError.hpp"
#include "CAENFlashError.hpp"
#include "CAENHVError.hpp"
#include "CAENVMEError.hpp"
#include "doctest.h"

#include <iostream>

using namespace CAEN;

TEST_CASE("Test CAEN Errors") {
  std::cout << "VME Errors" << std::endl;
  for (unsigned int i = 0; i != 6; ++i) {
    try {
      CAENVMEError(-i);
    } catch (std::exception &evet) {
      std::cout << evet.what() << std::endl;
    }
  }
  std::cout << "CAENComm Errors" << std::endl;
  for (unsigned int i = 0; i != 14; ++i) {
    try {
      CAENCommError(-i);
    } catch (std::exception &evet) {
      std::cout << evet.what() << std::endl;
    }
  }
  std::cout << "CAENHV Errors" << std::endl;
  for (int i = 0; i != 38; ++i) {
    try {
      if (i <= 30)
        CAENHVError(+i);
      else {
        int toto = (0x1000 + i);
        CAENHVError(+toto);
      }
    } catch (std::exception &evet) {
      std::cout << evet.what() << std::endl;
    }
  }
  std::cout << "CAENDigitizer Errors" << std::endl;
  for (unsigned int i = 0; i != 35; ++i) {
    try {
      CAENDigitizerError(-i);
    } catch (std::exception &evet) {
      std::cout << evet.what() << std::endl;
    }
  }
  std::cout << "CAENDPP Errors" << std::endl;
  for (unsigned int i = 100; i != 145; ++i) {
    try {
      CAENDPPError(-i);
    } catch (std::exception &evet) {
      std::cout << evet.what() << std::endl;
    }
  }
  std::cout << "CAENFlash Errors" << std::endl;
  for (unsigned int i = 0; i != 8; ++i) {
    try {
      CAENFlashError(-i);
    } catch (std::exception &evet) {
      std::cout << evet.what() << std::endl;
    }
  }
}
