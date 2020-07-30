#pragma once

#include "TObject.h"

#include <string>
#include <vector>

class Channel: public TObject
{
public:
  Channel();
  void                clear();
  double              RecordLength{0.0};
  int                 Number{0};
  std::string         Name{""};
  double              TriggerTimeTag{0.0};
  double              DCoffset{0.0};
  double              StartIndexCell{0.0};
  int                 Group{0};
  std::vector<double> Data;
  ClassDef(Channel, 2);
};
