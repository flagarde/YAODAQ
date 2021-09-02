#include "Channel.hpp"

ClassImp(Channel);

Channel::Channel() {}

void Channel::clear()
{
  RecordLength   = 0.0;
  Number         = 0.0;
  TriggerTimeTag = 0.0;
  DCoffset       = 0.0;
  StartIndexCell = 0.0;
  Group          = 0.0;
  Data.clear();
}
