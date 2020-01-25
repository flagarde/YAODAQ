#ifndef BOARD_HPP
#define BOARD_HPP
#include <string>
#include "Connector.hpp"
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
  toml::value m_ConnectorConf;
  virtual void LoadConfig() final;
  std::shared_ptr<Connector> m_Connector;
  std::int32_t m_Handle{-1};
};

#endif
