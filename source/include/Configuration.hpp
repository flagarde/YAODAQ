#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP
#include <string>
#include <vector>
#include <map>
#include "BoardInfos.hpp"
#include "value.hpp"

class Configuration 
{
public:
  void parse();
  std::string getFileName();
  void setFileName(const std::string&);
private:
  void checkFile();
  void fillIndexes();
  std::string m_Filename{""};
  toml::value m_Conf;
  std::vector<std::string> m_Room_Names;
  std::vector<std::string> m_Rack_Names;
  std::vector<std::string> m_Crate_Names;
  std::map<std::string,BoardInfos> m_BoardsInfos;
};

#endif
