#include "Configuration.hpp"

#include "Exception.hpp"
#include "Internal.hpp"
#include "StatusCode.hpp"
#include "spdlog.h"
#include "toml.hpp"

#include <algorithm>
#include <iostream>

int Configuration::m_ConnectorID = 1;

toml::value Configuration::getConfig(const std::string& module)
{
  if(m_ModuleConfig.find(module) == m_ModuleConfig.end()) { throw Exception(StatusCode::NOT_FOUND, "Board/Module " + module + " not found in configuration !"); }
  else
    return m_ModuleConfig[module];
}

ConnectorInfos Configuration::getConnectorInfos(const std::string& module)
{
  if(m_ConnectorInfos.find(module) == m_ConnectorInfos.end()) { throw Exception(StatusCode::NOT_FOUND, "Board " + module + " not found in configuration !"); }
  return m_ConnectorInfos[module];
}

void Configuration::parse()
{
  if(m_Filename == "") { throw Exception(StatusCode::NOT_FOUND, "No Configuration file given !"); }
  if(m_isParsed == false)
  {
    m_Conf = toml::parse<toml::preserve_comments, std::map, std::vector>(m_Filename);
    parseRooms();
    fillIndexes();
    m_isParsed = true;
  }
}

void Configuration::throwIfExists(std::vector<std::string>& type, const std::string& typeName, const std::string& name)
{
  if(std::find(type.begin(), type.end(), name) != type.end()) { throw Exception(StatusCode::ALREADY_PRESENT, typeName + " name \"" + name + "\" is already taken"); }
  else
    type.push_back(name);
}

void Configuration::parseRooms()
{
  for(const auto& room: toml::find<toml::array>(m_Conf, "Room"))
  {
    actualRoomName = toml::find_or<std::string>(room, "Name", "");
    if(actualRoomName == "") spdlog::warn("No Room table");
    throwIfExists(m_Room_Names, "Room", actualRoomName);
    parseRacks(room);
  }
}

void Configuration::parseRacks(const toml::value& room)
{
  for(const auto& rack: toml::find<toml::array>(room, "Rack"))
  {
    actualRackName = toml::find_or<std::string>(rack, "Name", "");
    if(actualRackName == "") spdlog::warn("No Rack table");
    throwIfExists(m_Rack_Names, "Rsck", actualRackName);
    parseCrates(rack);
  }
}

void Configuration::parseCrates(const toml::value& rack)
{
  for(const auto& crate: toml::find<toml::array>(rack, "Crate"))
  {
    bool crateHaveConnector{true};
    actualCrateName = toml::find_or<std::string>(crate, "Name", "");
    if(actualCrateName == "") spdlog::warn("No Crate table");
    throwIfExists(m_Crate_Names, "Crate", actualCrateName);
    toml::value crate_connector{};
    try
    {
      ++m_ConnectorID;
      m_CrateConnectorID = m_ConnectorID;
      crate_connector    = toml::find<toml::table>(crate, "Connector");
    }
    catch(const std::out_of_range& e)
    {
      crateHaveConnector = false;
    }
    parseModules(crate, crate_connector, crateHaveConnector);
  }
}

void Configuration::parseModules(const toml::value& crate, const toml::value& crateConnectorParameters, bool haveCrateConnector)
{
  int         boardConnectorID{0};
  toml::value boardConnectorParameters{};
  for(const auto& board: toml::find<toml::array>(crate, "Board"))
  {
    std::string moduleName = toml::find_or<std::string>(board, "Name", "");
    if(moduleName == "")
    {
      spdlog::error("Board have no Name");
      throw Exception(StatusCode::NOT_FOUND, "Board \"" + moduleName + "\" doesn't have a \"Name\" key");
    }
    throwIfExists(m_Module_Names, "Module", moduleName);
    std::string type = toml::find_or<std::string>(board, "Type", "");
    if(type == "") { throw Exception(StatusCode::NOT_FOUND, "Board \"" + moduleName + "\" doesn't have a \"Type\" key"); }
    try
    {
      boardConnectorParameters = toml::find<toml::table>(board, "Connector");
      ++m_ConnectorID;
      boardConnectorID = m_ConnectorID;
    }
    catch(const std::out_of_range& e)
    {
      if(haveCrateConnector == false)
      { throw Exception(StatusCode::NOT_FOUND, "Board \"" + moduleName + "\" doesn't have a Connector and the Crate \"" + actualCrateName + "\" in which is plugged to doesn't have one neither"); }
      else
      {
        boardConnectorParameters = crateConnectorParameters;
        boardConnectorID         = m_CrateConnectorID;
      }
    }
    Infos infos(actualRoomName, actualRackName, actualCrateName, moduleName, type);
    m_BoardsInfos.emplace(moduleName, BoardInfos(infos, board, boardConnectorParameters));
    m_ModuleConfig.emplace(moduleName, board);
    m_ConnectorInfos.emplace(moduleName, ConnectorInfos(boardConnectorParameters, haveCrateConnector, boardConnectorID));
  }
}

void Configuration::fillIndexes()
{
  std::sort(m_Room_Names.begin(), m_Room_Names.end());
  std::sort(m_Rack_Names.begin(), m_Rack_Names.end());
  std::sort(m_Crate_Names.begin(), m_Crate_Names.end());
  for(std::map<std::string, BoardInfos>::iterator it = m_BoardsInfos.begin(); it != m_BoardsInfos.end(); ++it)
  {
    static int boardindex = 0;
    it->second.setRoomIndex(std::distance(m_Room_Names.begin(), std::find(m_Room_Names.begin(), m_Room_Names.end(), it->second.getRoomName())));
    it->second.setRackIndex(std::distance(m_Rack_Names.begin(), std::find(m_Rack_Names.begin(), m_Rack_Names.end(), it->second.getRackName())));
    it->second.setCrateIndex(std::distance(m_Crate_Names.begin(), std::find(m_Crate_Names.begin(), m_Crate_Names.end(), it->second.getCrateName())));
    it->second.setIndex(boardindex);
    ++boardindex;
  }
}

std::string Configuration::getFileName()
{
  return m_Filename;
}

void Configuration::setFileName(const std::string& file)
{
  m_Filename = file;
}
