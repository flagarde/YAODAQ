#include "GeneralParameters.hpp"

namespace yaodaq
{

  std::string GeneralParameters::m_Host{"127.0.0.1"};

  int         GeneralParameters::m_Port{8080};

  std::string GeneralParameters::m_URL{"ws://127.0.0.1:8080/"};

  bool        GeneralParameters::m_URLIsSet{false};



  void        GeneralParameters::setHost(const std::string& host)
  {
    m_Host=host;
    m_URLIsSet =  true;
  }

  std::string GeneralParameters::getHost()
  {
    return m_Host;
  }

  void        GeneralParameters::setPort(const int& port)
  {
    m_Port=port;
    m_URLIsSet =  true;
  }

  int         GeneralParameters::getPort()
  {
    return m_Port;
  }

  void GeneralParameters::setURL(const std::string& url)
  {
    m_URL = url;
    m_URLIsSet =  true;
  }

  std::string GeneralParameters::getURL()
  {
    if(m_URLIsSet) return m_URL;
    else
    {
      return "ws://"+m_Host+":"+std::to_string(m_Port)+"/";
    }
  }

};
