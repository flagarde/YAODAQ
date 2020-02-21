#pragma once

#include "TObject.h"

#include <string>

class Channel : public TObject {
public:
  Channel();
  void clear() {
    RecordLength = 0.0;
    Number = 0.0;
    TriggerTimeTag = 0.0;
    DCoffset = 0.0;
    StartIndexCell = 0.0;
    Group = 0.0;
    Data.clear();
    Time.clear();
  }
  double RecordLength{0.0};
  double Number{0.0};
  std::string Name{""};
  double TriggerTimeTag{0.0};
  double DCoffset{0.0};
  double StartIndexCell{0.0};
  double Group{0.0};
  std::vector<double> Data;
  std::vector<double> Time;
  ClassDef(Channel, 1); // Simple event class
};
