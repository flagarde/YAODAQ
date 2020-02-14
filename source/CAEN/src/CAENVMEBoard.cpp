#include "CAENVMEBoard.hpp"
#include "CAENVMEError.hpp"
#include "CAENVMElib.h"

namespace CAEN
{

CAENVMEBoard::CAENVMEBoard(const std::string& name):Board("CAENVME",name)
{
    
}

std::string CAENVMEBoard::SWRelease()
{
    char swrelease[100];
    CAENVMEError(CAENVME_SWRelease(swrelease));
    return swrelease;
}

}
