#pragma once
#include "Event.hpp"
#include "RootFile.hpp"
#include "TTree.h"
#include "ElogManager.hpp"

class DAQFile: public RootFile
{
public:
  DAQFile(const std::string& name, const std::string& option = "", const std::string& title = "", const int& compress = ROOT::RCompressionSetting::EDefaults::kUseCompiledDefault, const int& netopt = 0);
  virtual ~DAQFile();
  virtual void parseData(const Data& data) final;
  virtual void doAfterOpen() final;
  virtual void doBeforeClose() final;

protected:
  virtual void setID() final;

private:
  TTree* m_Tree{nullptr};
  Event* m_Event{nullptr};
  elogpp::ElogManager m_ElogManager;
};
