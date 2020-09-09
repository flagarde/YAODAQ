#include "ConfigurationLoader.hpp"

#include "Exception.hpp"
#include "Internal.hpp"
#include "StatusCode.hpp"
#include "toml.hpp"

int ConfigurationLoader::m_ConnectorID = 1;

void ConfigurationLoader::parse()
{
  std::lock_guard<std::mutex> guard(m_Mutex);
  if(!m_HaveBeenParsed.load())
  {
    if(m_Filename == "") { throw Exception(StatusCode::NOT_FOUND, "No Configuration file given !"); }
    m_Conf = toml::parse<toml::preserve_comments, std::map, std::vector>(m_Filename);
    parseRooms();
    fillIndexes();
    m_HaveBeenParsed = true;
  }
}

void ConfigurationLoader::parseRooms()
{
  for(const auto& room: toml::find<toml::array>(m_Conf, "Room"))
  {
    try
    {
      actualRoomName = toml::find<std::string>(room, "Name");
    }
    catch(const std::out_of_range& e)
    {
      throw Exception(StatusCode::NOT_FOUND, "Room should have a name !\n" + std::string(e.what()));
    }
    throwIfExists(m_Room_Names, "Room", actualRoomName);
    parseRacks(room);
  }
}

void ConfigurationLoader::parseRacks(const toml::value& room)
{
  for(const auto& rack: toml::find<toml::array>(room, "Rack"))
  {
    try
    {
      actualRackName = toml::find<std::string>(rack, "Name");
    }
    catch(const std::out_of_range& e)
    {
      throw Exception(StatusCode::NOT_FOUND, "Rack should have a name !\n" + std::string(e.what()));
    }
    throwIfExists(m_Rack_Names, "Rack", actualRackName);
    parseCrates(rack);
  }
}

void ConfigurationLoader::parseCrates(const toml::value& rack)
{
  for(const auto& crate: toml::find<toml::array>(rack, "Crate"))
  {
    bool crateHaveConnector{true};
    try
    {
      actualCrateName = toml::find<std::string>(crate, "Name");
    }
    catch(const std::out_of_range& e)
    {
      throw Exception(StatusCode::NOT_FOUND, "Crate should have a name !\n" + std::string(e.what()));
    }
    throwIfExists(m_Crate_Names, "Crate", actualCrateName);
    try
    {
      toml::value crate_connector = toml::find<toml::table>(crate, "Connector");
      ++m_ConnectorID;
      m_CrateConnectorID = m_ConnectorID;
      parseBoards(crate,crate_connector,true);
    }
    catch(const std::out_of_range& e)
    {
      parseBoards(crate,{},false);
      parseModules(crate);
    }
  }
}

void ConfigurationLoader::parseBoards(const toml::value& crate, const toml::value& crateConnectorParameters, bool haveCrateConnector)
{
  int         boardConnectorID{0};
  toml::value boardConnectorParameters{};
  for(const auto& board: toml::find<toml::array>(crate, "Board"))
  {
    std::string moduleName{""};
    try
    {
      moduleName = toml::find<std::string>(board, "Name");
    }
    catch(const std::out_of_range& e)
    {
      throw Exception(StatusCode::NOT_FOUND, "Board should have a name !\n" + std::string(e.what()));
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
    Infos infos(actualRoomName, actualRackName, actualCrateName, moduleName, type,Category::Board);
    m_BoardsInfos.emplace(moduleName, BoardInfos(infos, board, boardConnectorParameters));
    m_ConnectorInfos.emplace(moduleName, ConnectorInfos(boardConnectorParameters, haveCrateConnector, boardConnectorID));
  }
}


void ConfigurationLoader::clear()
{
  m_Room_Names.clear();
  m_Rack_Names.clear();
  m_Crate_Names.clear();
  m_Module_Names.clear();
  m_BoardsInfos.clear();
  m_ConnectorInfos.clear();
  m_ConnectorID      = 1;
  m_CrateConnectorID = 0;
  m_HaveBeenParsed   = false;
}

ConnectorInfos ConfigurationLoader::getConnectorInfos(const std::string& module)
{
  if(m_ConnectorInfos.find(module) == m_ConnectorInfos.end()) { throw Exception(StatusCode::NOT_FOUND, "Board " + module + " not found in configuration !"); }
  return m_ConnectorInfos[module];
}


void ConfigurationLoader::throwIfExists(std::vector<std::string>& type, const std::string& typeName, const std::string& name)
{
  if(std::find(type.begin(), type.end(), name) != type.end()) { throw Exception(StatusCode::ALREADY_PRESENT, typeName + " name \"" + name + "\" is already taken"); }
  else type.push_back(name);
}


toml::value ConfigurationLoader::getConfig(const std::string& name)
{
  if(m_BoardsInfos.find(name) == m_BoardsInfos.end()) { throw Exception(StatusCode::NOT_FOUND, "Board " + name + " not found in configuration !"); }
  return m_BoardsInfos[name].getParameters();
}

void ConfigurationLoader::parseModules(const toml::value& crate)
{
  int         ModulesConnectorID{0};
  toml::value ModulesConnectorParameters{};
  for(const auto& board: toml::find<toml::array>(crate, "Module"))
  {
    std::string moduleName{""};
    try
    {
      moduleName = toml::find<std::string>(board, "Name");
    }
    catch(const std::out_of_range& e)
    {
      throw Exception(StatusCode::NOT_FOUND, "Module should have a name !\n" + std::string(e.what()));
    }
    throwIfExists(m_Module_Names, "Module", moduleName);
    std::string type = toml::find_or<std::string>(board, "Type", "");
    if(type == "") { throw Exception(StatusCode::NOT_FOUND, "Module \"" + moduleName + "\" doesn't have a \"Type\" key"); }
    Infos infos(actualRoomName, actualRackName, actualCrateName, moduleName, type,Category::Module);
    m_BoardsInfos.emplace(moduleName, BoardInfos(infos, board));
  }
}

void ConfigurationLoader::reloadParameters(const std::string& name)
{
  m_Conf = toml::parse<toml::preserve_comments, std::map, std::vector>(m_Filename);
  for(const auto& room: toml::find<toml::array>(m_Conf, "Room"))
  {
    for(const auto& rack: toml::find<toml::array>(room, "Rack"))
    {
      for(const auto& crate: toml::find<toml::array>(rack, "Crate"))
      {
        for(const auto& board: toml::find<toml::array>(crate, m_BoardsInfos[name].getCategory()))
        {
          //Should not need to check this because we check it before but the user could do mistake and erase this part
          try
          {
            std::string boardName = toml::find<std::string>(board, "Name");
            if(boardName==name)
            {
              m_BoardsInfos[name].setParameters(board);
              return;
            }
          }
          catch(const std::out_of_range& e)
          {
          }
        }
      }
      // If it can go there that means the module or board as been erased from the config file !! BAD boys
      throw Exception(StatusCode::NOT_FOUND, m_BoardsInfos[name].getCategory()+" \"" + name + "\" has been erased from the configuration file !");
    }
  }
}

void ConfigurationLoader::reloadConnectorParameters(const std::string& name)
{
  m_Conf = toml::parse<toml::preserve_comments, std::map, std::vector>(m_Filename);
  for(const auto& room: toml::find<toml::array>(m_Conf, "Room"))
  {
    for(const auto& rack: toml::find<toml::array>(room, "Rack"))
    {
      for(const auto& crate: toml::find<toml::array>(rack, "Crate"))
      {
        // Try to find the board
        for(const auto& board: toml::find<toml::array>(crate, "Board"))
        {
          //Should not need to check this because we check it before but the user could do mistake and erase this part
          try
          {
            std::string boardName = toml::find<std::string>(board, "Name");
            if(boardName==name)
            {
              try
              {
                m_BoardsInfos[name].setConnectorParameters(toml::find<toml::table>(board, "Connector"));
                return;
              }
              catch(const std::out_of_range& e)
              {
                m_BoardsInfos[name].setConnectorParameters(toml::find<toml::table>(crate, "Connector"));
              }
            }
          }
          catch(const std::out_of_range& e)
          {
            throw Exception(StatusCode::NOT_FOUND, "Board \"" + name + "\" doesn't have a connector even does the rack it depends ! connector has been erased from the configuration file !");
          }
        }
      }
    }
  }
}


void ConfigurationLoader::fillIndexes()
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

std::string ConfigurationLoader::getFileName()
{
  return m_Filename;
}

void ConfigurationLoader::setFileName(const std::string& file)
{
  m_Filename = file;
}
