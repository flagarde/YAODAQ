#ifndef CHANNEL_H
#define CHANNEL_H
#include "TObject.h"
#include <string>

class Channel : public TObject {
public:
  Channel();
  double RecordLength{0};
  double Number{0};
  std::string Name{""};
  double TriggerTimeTag;
  double DCoffset;
  double StartIndexCell;
  double Group{0};
  std::vector<double> Data;
  ClassDef(Channel, 1); // Simple event class
};
#endif
