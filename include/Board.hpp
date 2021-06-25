#pragma once

#include "Connector.hpp"
#include "DumpConnector.hpp"
#include "ConnectorFactory.hpp"
#include "Module.hpp"

#include <memory>
#include <string>

namespace yaodaq
{

class Board: public Module
{
public:
  Board(const std::string& name = "", const std::string& type = "DefaultBoard", const yaodaq::CLASS& _class = yaodaq::CLASS::Board);
  virtual ~Board() = default;
  void printConnectorParameters();
  const std::shared_ptr<Connector>& getConnector();

private:
  static ConnectorFactory    m_ConnectorFactory;
  std::shared_ptr<Connector> m_Connector{std::make_shared<DumpConnector>()};
  void               CallBoardDisconnect() final;
  void               CallBoardConnect() final;
  virtual void               DoConnect();
  virtual void               DoDisconnect();
  Board() = delete;
  bool m_IsConnected{false};

protected:
  std::int32_t m_Handle{-1};
};

}
