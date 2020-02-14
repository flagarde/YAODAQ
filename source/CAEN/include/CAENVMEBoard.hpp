#ifndef CAENVMEBOARD_HPP
#define CAENVMEBOARD_HPP
#include "Board.hpp"

namespace CAEN
{

class CAENVMEBoard : public Board
{
public:
    CAENVMEBoard(const std::string& name="Unknown");
    
    std::string SWRelease();
    
    
private:
    

};

}
#endif
