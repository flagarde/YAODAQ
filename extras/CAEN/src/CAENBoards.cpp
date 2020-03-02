#include "PluginRegistry.hpp"

#include "CAENVMEBoard.hpp"
#include "CAENDigitizerBoard.hpp"
#include "CAENCommBoard.hpp"

FlakedTuna::PluginRegistry* pr = nullptr;                            
extern "C" FT_EXPORT FlakedTuna::PluginRegistry* GetPluginRegistry() 
{                                                                    
  pr = new FlakedTuna::PluginRegistry();
  pr->RegisterPlugin<CAEN::CAENCommBoard, Board>();
  pr->RegisterPlugin<CAEN::CAENDigitizerBoard, Board>();
  pr->RegisterPlugin<CAEN::CAENVMEBoard, Board>();
  return pr;                                      
} 
  
extern "C" FT_EXPORT void ClosePluginRegistry() 
{                                               
  if(pr != nullptr) { delete pr; }              
}
  
FLAKED_TUNA_PLUGIN_VERSION(1.0)
