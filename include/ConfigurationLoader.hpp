#pragma once

#include "BoardInfos.hpp"
#include "ConnectorInfos.hpp"
#include "toml.hpp"

#include <map>
#include <string>
#include <vector>
#include <atomic>
#include <mutex>

namespace yaodaq
{

class ConfigurationLoader
{
public:
  void           parse();
  void           reparseModule();
  std::string    getFileName();
  void           setFileName(const std::string&);
  toml::value    getConfig(const std::string&);
  ConnectorInfos getConnectorInfos(const std::string&);
  BoardInfos     getBoardInfos(const std::string&);
  void           clear();
  /* Reload only the Module/Board parameters */
  void           reloadParameters(const std::string&);
  /* Reload only the Board connector parameters */
  void           reloadConnectorParameters(const std::string&);
private:
  std::mutex                            m_Mutex;
  static int                            m_ConnectorID;
  std::atomic<bool>                     m_HaveBeenParsed{false};
  int                                   m_CrateConnectorID{0};
  void                                  parseRooms();
  void                                  parseRacks(const toml::value& room);
  void                                  parseCrates(const toml::value& rack);
  void                                  parseModules(const toml::value& crate);
  void                                  parseBoards(const toml::value& crate, const toml::value& connectorParameters, bool haveCrateConnector);
  void                                  throwIfExists(std::vector<std::string>& type, const std::string& typeName, const std::string& name);
  void                                  fillIndexes();
  std::string                           m_Filename{""};
  toml::value                           m_Conf;
  std::vector<std::string>              m_Room_Names;
  std::vector<std::string>              m_Rack_Names;
  std::vector<std::string>              m_Crate_Names;
  std::vector<std::string>              m_Module_Names;
  std::map<std::string, BoardInfos>     m_BoardsInfos;
  std::map<std::string, ConnectorInfos> m_ConnectorInfos;
  std::string                           actualRoomName{""};
  std::string                           actualRackName{""};
  std::string                           actualCrateName{""};
};

}
