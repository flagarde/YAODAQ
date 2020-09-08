#pragma once

#include <string>

class GeneralParameters
{
public:
  GeneralParameters() = default;
  static void        setURL(const std::string& url) { m_URL = url; }
  static std::string getURL() { return m_URL; }

private:
  static std::string m_URL;
};
