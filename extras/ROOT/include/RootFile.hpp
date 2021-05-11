#pragma once

#include "File.hpp"
#include "TFile.h"

#include <memory>

class RootFile: public File
{
public:
  RootFile(const std::string& name, const std::string& option = "", const std::string& title = "", const int& compress = ROOT::RCompressionSetting::EDefaults::kUseCompiledDefault, const int& netopt = 0);
  void open() final;
  void close() final;
  bool isOpened() final;
  virtual void parseData(const Data&);
  virtual ~RootFile();

protected:
  virtual void setID();

private:
  std::string m_Title{""};
  int         m_CompressionLevel{ROOT::RCompressionSetting::EDefaults::kUseCompiledDefault};
  int         m_NetOpt{0};
  TFile*      m_File{nullptr};
};
