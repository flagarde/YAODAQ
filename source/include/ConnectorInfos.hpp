#pragma once

#include "toml.hpp"

class ConnectorInfos
{
public:
  ConnectorInfos(const toml::value& params = {}, const bool& isShared = false,
                 const int& id = -1);
  bool         isSharedConnector();
  bool         isSharedConnector() const;
  int          getID();
  int          getID() const;
  toml::value& getParameters();
  toml::value  getParameters() const;
  void         printParameters();
  void         printParameters() const;

private:
  toml::value m_Parameters;
  bool        m_IsSharedConnector{false};
  int         m_ID{-1};
};
