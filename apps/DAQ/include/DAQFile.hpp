#pragma once
#include "Event.hpp"
#include "RootFile.hpp"
#include "TTree.h"
//#include "ElogManager.hpp"

class DAQFile: public RootFile
{
public:
  DAQFile(const std::string& name, const std::string& option = "", const std::string& title = "", const int& compress = ROOT::RCompressionSetting::EDefaults::kUseCompiledDefault, const int& netopt = 0);
  virtual ~DAQFile();
  void parseData(const yaodaq::Data& data) final;
  void doAfterOpen() final;
  void doBeforeClose() final;
protected:
  void setID() final;
  void createElog();
  void updateElog();
private:
  TTree* m_Tree{nullptr};
  Event* m_Event{nullptr};
  //elogpp::ElogManager m_ElogManager;
};
