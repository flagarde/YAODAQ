#include "Event.hpp"

ClassImp(Event);
Event::Event() {}

void Event::addChannel(const Channel& ch)
{
  Channels.push_back(ch);
}

void Event::clear()
{
  BoardID        = 0;
  EventNumber    = 0;
  Pattern        = 0;
  ChannelMask    = 0;
  EventSize      = 0;
  TriggerTimeTag = 0;
  Period_ns      = 0;
  Model          = "";
  FamilyCode     = "";
  Channels.clear();
}
