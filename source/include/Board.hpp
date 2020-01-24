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
  virtual void Connect();
  virtual void Disconnect();
  virtual ~Board(){};
private:
  std::shared_ptr<Connector> m_Connector;
  std::int32_t m_Handle{-1};
};

#endif
