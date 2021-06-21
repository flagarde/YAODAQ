#include "File.hpp"

#include <filesystem>
#include <iostream>

namespace yaodaq
{

File::File(const std::string& name, const std::string& option): m_Name(name), m_Option(option) {}

void File::setName(const std::string& name)
{
  m_Name = name;
}

void File::setOptions(const std::string& option)
{
  m_Option = option;
}

std::string File::getDirectory()
{
  setPath();
  std::filesystem::path ret = m_Path;
  ret.remove_filename();
  return ret.string();
}

std::string File::getName()
{
  setPath();
  return m_Path.filename().string();
}

std::string File::getExtension()
{
  setPath();
  return m_Path.extension().string();
}

std::string File::getPath()
{
  setPath();
  return m_Path.string();
}

std::string File::fillvariables(const std::string& str, const std::string variable, const std::string& value)
{
  std::string ret = str;
  while(ret.find(variable) != std::string::npos) { ret.replace(ret.find(variable), variable.size(), value); }
  return ret;
}

void File::setPath()
{
  if(m_Path.empty())
  {
    setID();
    m_Path = fillvariables(m_Name, "${ID}", m_ID);
    createDirectories();
  }
}

void File::createDirectories()
{
  std::filesystem::directory_entry directories{getDirectory()};
  if(!directories.exists() && directories.path() != "") std::filesystem::create_directories(directories);
}

}
