#include "FileWritter.hpp"

FileWritter::FileWritter(const std::string& name, const std::string& type): Module(name, type) {}

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

void FileWritter::DoOnData(const Data& data)
{
  //std::cout<<data.get()<<std::endl;
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
