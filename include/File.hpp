#pragma once

#include "Message.hpp"

#include <filesystem>
#include <string>

class File
{
public:
  File(const std::string& name = "", const std::string& option = "");
  void         setName(const std::string&);
  void         setOptions(const std::string&);
  std::string  getDirectory();
  std::string  getName();
  std::string  getExtension();
  std::string  getPath();
  virtual bool isOpened() = 0;
  virtual void open()     = 0;
  virtual void close()    = 0;
  virtual void doAfterOpen(){};
  virtual void doBeforeClose(){};
  virtual void parseData(const Data& data) = 0;
  virtual ~File()                          = default;
protected:
  std::string           fillvariables(const std::string& str, const std::string variable, const std::string& value);
  virtual void          setID() = 0;
  void                  setPath();
  void                  createDirectories();
  std::string           m_Name;
  std::filesystem::path m_Path;
  std::string           m_ID{"-1"};
  std::string           m_Option;

};
