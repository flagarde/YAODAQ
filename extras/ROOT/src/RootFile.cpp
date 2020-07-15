#include "RootFile.hpp"
#include <iostream>

RootFile::RootFile(const std::string& name,const std::string& option,const std::string& title,const int& compress,const int& netopt):File(name,option)
{
  m_Title=title;
  m_CompressionLevel=compress;
  m_NetOpt=netopt;
}

void RootFile::open()
{
  if(!isOpened())
  {
    setID();
    m_Title=fillvariables(m_Title,"${ID}",m_ID);
    m_File=TFile::Open(getPath().c_str(),m_Option.c_str(),m_Title.c_str(),m_CompressionLevel,m_NetOpt);
  }
}
 
void RootFile::close()
{
  if(isOpened())
  {
    m_File->Flush();
    m_File->Write();
    m_File->Close();
  }
}

void RootFile::setID()
{
}

bool RootFile::isOpened()
{
  if(m_File==nullptr) return false;
  else return m_File->IsOpen();
}

RootFile::~RootFile()
{
  close();
  if(m_File!=nullptr) delete m_File;
}

void RootFile::parseData(const Data&){}
