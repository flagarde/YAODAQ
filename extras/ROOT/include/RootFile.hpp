#pragma once

#include "File.hpp"
#include "TFile.h"

#include <memory>

class RootFile : public File
{
public:
  virtual void open() final;
  virtual void close() final;
  virtual bool isOpened() final;
  std::unique_ptr<TFile> m_File{nullptr};
  
};
