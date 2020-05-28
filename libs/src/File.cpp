#include "File.hpp"
#include <filesystem>

File::File(const std::string& name):m_Path(name)
{
}

std::string File::getPath()
{
  return  m_Path.remove_filename();
}

std::string File::getName()
{
  return m_Path.filename();
}

std::string File::getExtension()
{
  return m_Path.extension();
}
