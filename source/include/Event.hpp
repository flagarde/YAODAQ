#pragma once

#include "Channel.hpp"
#include "TObject.h"

#include <string>

class Event : public TObject {
public:
  Event();
  void addChannel(const Channel &ch) { Channels.push_back(ch); }
  void clear() {
    BoardID = 0;
    EventNumber = 0;
    Pattern = 0;
    Channels.clear();
  }
  double BoardID{0};
  int EventNumber{0};
  int Pattern{0};
  std::vector<Channel> Channels;
  ClassDef(Event, 1); // Simple event class
};
