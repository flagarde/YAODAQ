#include "ConnectorFactory.hpp"
#include <iostream>
#include "CAENVMEConnector.hpp"
#include "CAENDigitizerConnector.hpp"
#include "toml.hpp"

std::shared_ptr<Connector> ConnectorFactory::createConnector(const ConnectorInfos& infos)
{
  infos.printParameters();
  //First I need to see if a type is given to create it
  std::string m_Type{""};
  try
  {
    m_Type=toml::find<std::string>(infos.getParameters(),"Type");
  }
  catch(const std::out_of_range& e)
  {
    std::cout<<"Type key not set in Connector !"<<std::endl;
    std::exit(2);
  }
  if(m_Type=="CAENVME")
  {
    if(infos.isSharedConnector()&&m_SharedConnectors.find(infos.getID())==m_SharedConnectors.end())
    {
      m_SharedConnectors.emplace(infos.getID(),std::make_shared<CAEN::CAENVMEConnector>(infos));
      return m_SharedConnectors[infos.getID()];
    }
    else return std::make_shared<CAEN::CAENVMEConnector>(infos);
  }
  if(m_Type=="CAENDigitizer")
  {
    if(infos.isSharedConnector()&&m_SharedConnectors.find(infos.getID())==m_SharedConnectors.end())
    {
      m_SharedConnectors.emplace(infos.getID(),std::make_shared<CAEN::CAENDigitizerConnector>(infos));
      return m_SharedConnectors[infos.getID()];
    }
    else return std::make_shared<CAEN::CAENDigitizerConnector>(infos);
  }
  else 
  {
    std::cout<<"Connector Type not known or not loaded !"<<std::endl;
    std::exit(2);
  }
}
