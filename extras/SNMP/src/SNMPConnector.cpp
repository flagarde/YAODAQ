#include "SNMPConnector.hpp"

#define WIN32_LEAN_AND_MEAN
#include "net-snmp-config.h"
#include "net-snmp-includes.h"
#include "Exception.hpp"
#include "StatusCode.hpp"
#include <memory>

namespace SNMP
{
  
  SNMPConnector::SNMPConnector(): Connector("SNMP")
  {
  }
  
  void SNMPConnector::DoConnect()
  {
    SOCK_STARTUP;
    struct ::snmp_session session;
    snmp_sess_init(&session);                  // structure defaults
    session.version = m_version;
    session.peername = strdup(m_IP.c_str());
    session.community = (u_char *)strdup(m_readCommunity.c_str());
    session.community_len = strlen(m_readCommunity.c_str());
    session.timeout = m_timeout;   // timeout (us)
    session.retries = m_retries;        // retries
    if (!(m_session = snmp_open(&session))) 
    {
      int liberr{0};
      int syserr{0};
      char* errstr{nullptr};
      snmp_error(&session, &liberr, &syserr, &errstr);
      std::string ret=errstr;
      free(errstr);
      throw Exception(StatusCode::FAILURE,ret);
    }
  }
  
  void SNMPConnector::DoDisconnect()
  {
    snmp_close(m_session);
    m_session=nullptr;
    SOCK_CLEANUP;
  }
  
  void SNMPConnector::verifyParameters()
  {    
    try
    {
      m_MIBPath = toml::find<std::string>(getParameters(), "MIB_path");
    }
    catch(const std::out_of_range& e)
    {
      Exception(StatusCode::NOT_FOUND,"\"MIB_path\" key not set !");
    }
    try
    {
      m_IP = toml::find<std::string>(getParameters(), "IP");
    }
    catch(const std::out_of_range& e)
    {
      Exception(StatusCode::NOT_FOUND,"\"IP\" key not set !");
    }
    try
    {
      m_MIBFilename = toml::find<std::string>(getParameters(), "MIB_filename");
    }
    catch(const std::out_of_range& e)
    {
      Exception(StatusCode::NOT_FOUND,"\"MIB_filename\" key not set !");
    }
    m_timeout = toml::find_or<int>(getParameters(), "Timeout",300000);
    m_readCommunity =  toml::find_or<std::string>(getParameters(), "ReadCommunity","public");
    m_retries = toml::find_or<int>(getParameters(), "Retries",2);
    std::string version = toml::find_or<std::string>(getParameters(), "Version","v1");
    if(version!="v1"||version!="v2c"||version!="v3")
    {
      Exception(StatusCode::NOT_FOUND,"\"VERSION\" should be v1, v2c or v3 !");
    }
    if(version=="v1") m_version=SNMP_VERSION_1;
    else if(version=="v2c") m_version=SNMP_VERSION_2c;
    else m_version=SNMP_VERSION_3;
    init_snmp("");
    init_mib();  
    if(!read_module(m_MIBFilename.c_str())) 
    {   
      Exception(StatusCode::NOT_FOUND,"Unable to load SNMP MIB file "+m_MIBFilename+"!\n");
    }
  }
  
  Response SNMPConnector::sendCommand(const Command& command) const
  {
    if(command.getParameter("Direction")=="ERROR") throw Exception(StatusCode::NOT_FOUND,"No key Direction SEND or RECEIVE found in the command !");
    else if(command.getParameter("Direction")=="SEND")
    {
      //return sendInfos(Command);
    }
    else if(command.getParameter("Direction")=="RECEIVE")
    {
      //return receiveInfos(Command);
    }
    else throw Exception(StatusCode::INVALID_PARAMETER,"Key Direction must contain SEND or RECEIVE !");
  }
  
  
}  // namespace CAEN
