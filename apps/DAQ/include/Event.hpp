 
 #pragma once
 
 #include "Channel.hpp"
 #include "TObject.h"
 #include <vector>
 #include <string>
 
 class Event: public TObject
 {
 public:
   Event();
   void addChannel(const Channel& ch);
   void clear();
   double                  BoardID{0};
   int                     EventNumber{0};
   int                     Pattern{0};
   int                     ChannelMask{0};
   double                  EventSize{0};
   double                  TriggerTimeTag{0};
   double                  Period_ns{0.0};
   std::string             Model{""};
   std::string             FamilyCode{""};
   std::vector<Channel> Channels;
   ClassDef(Event, 2);
 };
