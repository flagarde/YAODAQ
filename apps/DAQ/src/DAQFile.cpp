#include "DAQFile.hpp"

#include "Channel.hpp"
#include "Event.hpp"

#include <iostream>
#include <thread>

void DAQFile::createElog()
{
  elogpp::ElogEntry entry  = m_ElogManager.createEntry();
  std::chrono::system_clock::time_point now         = std::chrono::system_clock::now();
  std::time_t                           currentTime = std::chrono::system_clock::to_time_t(now);
  std::chrono::nanoseconds              now2        = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch());
  long                                  second      = now2.count() / 1000000000;
  entry.setAttribute("Begin", std::to_string(second));
  entry.user("DAQ").to("NAS", "Runs").send("V");
}

void DAQFile::updateElog()
{
  elogpp::ElogEntry                             entry       = m_ElogManager.createEntry();
  std::chrono::system_clock::time_point now         = std::chrono::system_clock::now();
  std::time_t                           currentTime = std::chrono::system_clock::to_time_t(now);
  std::chrono::nanoseconds              now2        = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch());
  long                                  second      = now2.count() / 1000000000;
  entry.setAttribute("End", std::to_string(second));
  entry.user("DAQ").to("NAS", "Runs").edit(m_ID).send();
}

DAQFile::DAQFile(const std::string& name, const std::string& option, const std::string& title, const int& compress, const int& netopt): RootFile(name, option, title, compress, netopt) {}

DAQFile::~DAQFile()
{
  if(m_Event != nullptr) delete m_Event;
}

void DAQFile::parseData(const Data& data)
{
  Json::Value json        = data.getContentAsJson();
  m_Event->BoardID        = json["EventInfos"]["BoardID"].asDouble();
  m_Event->EventNumber    = json["EventInfos"]["EventCounter"].asInt();
  m_Event->Pattern        = json["EventInfos"]["Pattern"].asInt();
  m_Event->ChannelMask    = json["EventInfos"]["ChannelMask"].asInt();
  m_Event->EventSize      = json["EventInfos"]["EventSize"].asDouble();
  m_Event->TriggerTimeTag = json["EventInfos"]["TriggerTimeTag"].asDouble();
  m_Event->Period_ns      = json["EventInfos"]["Period_ns"].asDouble();
  m_Event->Model          = json["EventInfos"]["Model"].asString();
  m_Event->FamilyCode     = json["EventInfos"]["FamilyCode"].asString();
  for(int i = 0; i != json["Event"]["Groups"].size(); ++i)
  {
    for(int j = 0; j != json["Event"]["Groups"][i]["Channels"].size(); ++j)
    {
      Channel channel;
      channel.Number         = json["Event"]["Groups"][i]["Channels"][j]["Channel"].asInt();
      channel.Group          = json["Event"]["Groups"][i]["Group"].asInt();
      channel.RecordLength   = json["Event"]["Groups"][i]["Data"].size();
      channel.TriggerTimeTag = json["Event"]["Groups"][i]["TriggerTimeTag"].asDouble();
      channel.StartIndexCell = json["Event"]["Groups"][i]["StartIndexCell"].asDouble();
      channel.Data.reserve(channel.RecordLength);
      for(int k = 0; k != json["Event"]["Groups"][i]["Channels"][j]["Data"].size(); ++k) { channel.Data.push_back(json["Event"]["Groups"][i]["Channels"][j]["Data"][k].asDouble()); }
      m_Event->addChannel(channel);
      channel.clear();
    }
  }
  m_Tree->Fill();
  m_Tree->Print();
  m_Event->clear();
}
void DAQFile::doAfterOpen()
{
 // std::thread create(&DAQFile::createElog,this);
 // create.detach();
  m_Tree  = new TTree("Tree", "Tree");
  m_Event = new Event();
  m_Tree->Branch("Events", &m_Event, 10, 0);
}

void DAQFile::doBeforeClose()
{
  m_Event->clear();
 // std::thread upload(&DAQFile::updateElog,this);
//  upload.detach();
}

void DAQFile::setID()
{
  //TODO Should fix this in elogpp
 /* try
  {
    elogpp::ElogEntry entry  = m_ElogManager.createEntry();
    m_ID=entry.user("DAQ").to("NAS", "Runs").getLastID();
  }
  catch(const int& e)
  {
    m_ID="0";
  }*/
  m_ID=std::to_string(std::stoi(m_ID)+1);
}
