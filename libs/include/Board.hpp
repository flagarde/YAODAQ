#pragma once

#include "Connector.hpp"
#include "ConnectorFactory.hpp"
#include "Module.hpp"

#include <memory>
#include <string>

class Board: public Module
{
public:
  Board(const std::string& name = "", const std::string& type = "Board");
  virtual ~Board(){};
  void printConnectorParameters();

private:
  std::shared_ptr<Connector> m_Connector;
  virtual void               CallModuleDisconnect() final;
  virtual void               CallModuleConnect() final;
  virtual void               DoConnect();
  virtual void               DoDisconnect();
  Board() = delete;

protected:
  std::int32_t            m_Handle{-1};
  static ConnectorFactory m_ConnectorFactory;
};
