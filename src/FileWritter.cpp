#include "FileWritter.hpp"

#include "Classes.hpp"

namespace yaodaq
{

FileWritter::FileWritter(const std::string& name, const std::string& type): Module(name, type , yaodaq::CLASS::FileWritter )
{
  skipConfigFile();
}

void FileWritter::setFileName(const std::string& name)
{
  m_File->setName(name);
}

void FileWritter::setFile(std::unique_ptr<File>&& file)
{
  m_File = std::move(file);
}

void FileWritter::setFileOptions(const std::string& option)
{
  m_File->setOptions(option);
}

void FileWritter::onData(const Data& data)
{
  m_File->parseData(data);
}

void FileWritter::DoStart()
{
  if(!m_File->isOpened())
  {
    m_File->open();
    m_File->doAfterOpen();
  }
}

void FileWritter::DoStop()
{
  if(m_File->isOpened())
  {
    m_File->doBeforeClose();
    m_File->close();
  }
}

};
