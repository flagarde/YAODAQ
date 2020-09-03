#pragma once

#include "toml.hpp"

class ConnectorInfos
{
public:
  ConnectorInfos(const toml::value& params = {}, const bool& isShared = false, const int& id = -1);
  bool         isSharedConnector();
  bool         isSharedConnector() const;
  int          getID();
  int          getID() const;
  toml::value  getParameters();
  toml::value  getParameters() const;
  void         printParameters();
  void         printParameters() const;
  void         setHandle(const std::int32_t& handle);
  void         addBoardConnected();
  void         removeBoardConnected();
  int          getNumberBoardConnected();

private:
  toml::value  m_Parameters;
  bool         m_IsSharedConnector{false};
  int          m_ID{-1};
  int          m_BoardConnected{0};
  std::int32_t m_Handle{0};
};
