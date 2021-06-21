#pragma once
#include "Board.hpp"
#include "File.hpp"

#include <memory>

namespace yaodaq
{

class FileWritter: public Module
{
public:
  FileWritter(const std::string& name = "", const std::string& type = "Default");
  void setFile(std::unique_ptr<File>&& file);
  void setFileName(const std::string& name);
  void setFileOptions(const std::string& option);
  virtual ~FileWritter() = default;

protected:
  void onData(const Data&) override;
  void DoStart();
  void DoStop();

private:
  std::unique_ptr<File> m_File{nullptr};
};

}
