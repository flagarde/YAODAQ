#pragma once

#include "File.hpp"
#include "IO/LCWriter.h"

class LCIOFile: public File
{
public:
  LCIOFile(const std::string& name, const std::string& option = "", const std::string& title = "", const int& compress = 0);
  void open() final;
  void close() final;
  bool isOpened() final;
  virtual void parseData(const Data&);
  virtual ~LCIOFile();

protected:
  virtual void setID();

private:
  std::string   m_Title{""};
  int           m_CompressionLevel{0};
  IO::LCWriter* m_File{nullptr};
  bool          m_IsOpened{false};
};
