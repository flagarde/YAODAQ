#include "Configuration.hpp"
#include <iostream>
#include <algorithm>
#include "toml.hpp"

toml::value Configuration::getConfig(const std::string& mmodule)
{
  if(m_ModuleConfig.find(mmodule)==m_ModuleConfig.end())
  {
    std::cout<<"Board/Module "<<mmodule<<" not found in configuration !"<<std::endl;
    throw;
  }
  else return m_ModuleConfig[mmodule];
}
  
ConnectorInfos Configuration::getConnectorInfos(const std::string& mmodule)
{
  if(m_ConnectorInfos.find(mmodule)==m_ConnectorInfos.end())
  {
    std::cout<<"Board "<<mmodule<<" not found in configuration !"<<std::endl;
    throw;
  }
  return m_ConnectorInfos[mmodule];
}


void Configuration::parse()
{
  if(m_isParsed==false)
  {
    m_Conf=toml::parse<toml::preserve_comments,std::map,std::vector>(m_Filename);
    checkFile();
    fillIndexes();
    m_isParsed=true;
  }
}

void Configuration::checkFile()
{
  static int connector_ID{-1};
  for(const auto& rooms : toml::find<toml::array>(m_Conf,"Room"))
  {
    std::string room_name=toml::find_or<std::string>(rooms,"name","");
    if(room_name=="" || std::find(m_Room_Names.begin(),m_Room_Names.end(),room_name) != m_Room_Names.end()) 
    {
      std::cout<<"Room must have a (unique) name !"<<std::endl;
      std::exit(2);
    }
    else
    {
      m_Room_Names.push_back(room_name);
      //std::cout<<"Parsing Room : "<<room_name<<std::endl;
      for(const auto& racks : toml::find<toml::array>(rooms,"Rack"))
      {
        std::string rack_name=toml::find_or<std::string>(racks,"name","");
        if(rack_name==""|| std::find(m_Rack_Names.begin(),m_Rack_Names.end(),rack_name) != m_Rack_Names.end())  
        {
          std::cout<<"Rack must have a (unique) name !"<<std::endl;
          std::exit(2);
        }
        else
        {
          m_Rack_Names.push_back(rack_name);
          //std::cout<<"Parsing Rack : "<<rack_name<<std::endl;
          for(const auto& crates : toml::find<toml::array>(racks,"Crate"))
          {
            bool crate_have_connector{true};
            std::string crate_name=toml::find_or<std::string>(crates,"name","");
            if(crate_name=="" || std::find(m_Crate_Names.begin(),m_Crate_Names.end(),crate_name) != m_Crate_Names.end()) 
            {
              std::cout<<"Crate must have a (unique) name !"<<std::endl;
              std::exit(2);
            }
            else
            {
              m_Crate_Names.push_back(crate_name);
              toml::value crate_connector{};
              toml::value board_connector{};
              try
              {
                crate_connector = toml::find<toml::table>(crates,"Connector");
              }
              catch(const std::out_of_range& e)
              {
                crate_have_connector=false;
              }
              //std::cout<<"Parsing Crate : "<<crate_name<<std::endl;
              for(const auto& boards : toml::find<toml::array>(crates,"Board"))
              {
                std::string board_name=toml::find_or<std::string>(boards,"name","");
                if(board_name=="" || m_BoardsInfos.find(board_name) != m_BoardsInfos.end()) 
                {
                  std::cout<<"Board must have a (unique) name !"<<std::endl;
                  std::exit(2);
                }
                else
                {
                  try
                  {
                    board_connector = toml::find<toml::table>(boards,"Connector");
                  }
                  catch(const std::out_of_range& e)
                  {
                    if(crate_have_connector==false)
                    {
                      std::cout<<"Board "<<board_name<<" doesn't have a Connector and the Crate ("<<crate_name<<") which is plugged to doesn't have one neither"<<std::endl;
                      std::exit(2);
                    }
                    else
                    {
                      ++connector_ID;
                      board_connector=crate_connector;
                    }
                  }
                  m_BoardsInfos.emplace(board_name,BoardInfos(room_name,rack_name,crate_name,board_name,boards,board_connector));
                  m_ModuleConfig.emplace(board_name,boards);
                  m_ConnectorInfos.emplace(board_name,ConnectorInfos(board_connector,crate_have_connector,connector_ID));
                }
              }
            }
          }
        }
      }
    }
  }
}

void Configuration::fillIndexes()
{
  std::sort(m_Room_Names.begin(),m_Room_Names.end());
  std::sort(m_Rack_Names.begin(),m_Rack_Names.end());
  std::sort(m_Crate_Names.begin(),m_Crate_Names.end());
  for(std::map<std::string,BoardInfos>::iterator it=m_BoardsInfos.begin();it!=m_BoardsInfos.end();++it)
  {
    static int boardindex=0;
    it->second.setRoomIndex(std::distance(m_Room_Names.begin(),std::find(m_Room_Names.begin(),m_Room_Names.end(),it->second.getRoomName())));
    it->second.setRackIndex(std::distance(m_Rack_Names.begin(),std::find(m_Rack_Names.begin(),m_Rack_Names.end(),it->second.getRackName())));
    it->second.setCrateIndex(std::distance(m_Crate_Names.begin(),std::find(m_Crate_Names.begin(),m_Crate_Names.end(),it->second.getCrateName())));
    it->second.setBoardIndex(boardindex);
    ++boardindex;
  }
}

std::string Configuration::getFileName()
{
  return m_Filename;
}

void Configuration::setFileName(const std::string& file)
{
  m_Filename=file;
}


