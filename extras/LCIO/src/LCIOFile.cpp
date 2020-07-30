#include "LCIOFile.hpp"

#include "EVENT/LCIO.h"
#include "IOIMPL/LCFactory.h"

#include <iostream>

LCIOFile::LCIOFile(const std::string& name, const std::string& option, const std::string& title, const int& compress): File(name, option)
{
  m_Title            = title;
  m_CompressionLevel = compress;
  m_File             = IOIMPL::LCFactory::getInstance()->createLCWriter();
  m_File->setCompressionLevel(m_CompressionLevel);
}

void LCIOFile::open()
{
  if(!isOpened())
  {
    setID();
    m_Title = fillvariables(m_Title, "${ID}", m_ID);
    if(m_Option == "WRITE_APPEND" || m_Option == "LCIO::WRITE_APPEND") m_File->open(m_Title.c_str(), EVENT::LCIO::WRITE_APPEND);
    else
      m_File->open(m_Title.c_str(), EVENT::LCIO::WRITE_NEW);
    m_IsOpened = true;
  }
}

void LCIOFile::close()
{
  if(isOpened())
  {
    m_File->flush();
    m_File->close();
    m_IsOpened = false;
  }
}

void LCIOFile::setID() {}

bool LCIOFile::isOpened()
{
  return m_IsOpened;
}

LCIOFile::~LCIOFile()
{
  close();
  if(m_File != nullptr) delete m_File;
}

void LCIOFile::parseData(const Data&) {}
