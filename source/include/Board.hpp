#ifndef BOARD_HPP
#define BOARD_HPP
#include <string>
#include "Connector.hpp"
#include "ConnectorFactory.hpp"
#include "Module.hpp"
#include <memory>

class Board : public Module
{
public:
  Board(const std::string& type="Unknown",const std::string& name="Unknown");
  virtual void DoConnect();
  virtual void DoDisconnect();
  virtual void DoInitialize();
  virtual ~Board(){};
  void printConnectorParameters();
private:
  virtual void DoDoConnect() final;
  virtual void DoDoDisconnect() final;
  virtual void LoadConfig() final;
  static ConnectorFactory m_ConnectorFactory;
  std::shared_ptr<Connector> m_Connector;
  std::int32_t m_Handle{-1};
};

#endif
