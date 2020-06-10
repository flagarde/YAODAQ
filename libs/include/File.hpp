#pragma once

#include <filesystem>
#include <string>

class File
{
public:
  File() = default;
  File(const std::string& name);
  std::string  getPath();
  std::string  getName();
  std::string  getExtension();
  virtual bool isOpened() = 0;
  virtual void open()     = 0;
  virtual void close()    = 0;

private:
  std::filesystem::path m_Path;
  std::string           m_ID{""};
};
