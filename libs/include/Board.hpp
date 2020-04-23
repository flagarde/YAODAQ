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
  virtual void DoConnect();
  virtual void DoDisconnect();
  virtual void DoInitialize();
  virtual ~Board(){};
  void printConnectorParameters();

private:
  virtual void               DoDoConnect() final;
  virtual void               DoDoDisconnect() final;
  static ConnectorFactory    m_ConnectorFactory;
  std::shared_ptr<Connector> m_Connector;

protected:
  std::int32_t m_Handle{-1};
};
