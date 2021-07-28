#pragma once

#include <string>

namespace yaodaq
{

  class GeneralParameters
  {
  public:
    GeneralParameters();
    static void        setHost(const std::string&);
    static std::string getHost();
    static void        setPort(const int& port);
    static int         getPort();
    static void        setURL(const std::string& url);
    static std::string getURL();
    static std::string getConfigsPath();
    static std::string getPluginsPath();
    static std::string getDataPath();
    static std::string getConfigFileName();
    static void reloadParameters(const char*);
  private:
    static std::string m_Host;
    static int         m_Port;
    static std::string m_URL;
    static bool        m_URLIsSet;
    static std::string m_ConfigsPath;
    static std::string m_PluginsPath;
    static std::string m_DataPath;
    static std::string m_ConfigFileName;
  };

}
