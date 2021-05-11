#pragma once

#include <string>

namespace yaodaq
{

  class GeneralParameters
  {
  public:
    static void        setHost(const std::string&);
    static std::string getHost();
    static void        setPort(const int& port);
    static int         getPort();
    static void        setURL(const std::string& url);
    static std::string getURL();

  private:
    static std::string m_Host;
    static int         m_Port;
    static std::string m_URL;
    static bool        m_URLIsSet;
  };

};
