#include "SNMPConnector.hpp"

#define WIN32_LEAN_AND_MEAN
#include "Exception.hpp"
#include "StatusCode.hpp"
#include "net-snmp/net-snmp-config.h"
#include "net-snmp/net-snmp-includes.h"
#include "Message.hpp"

#include <memory>
#include <bitset>

namespace SNMP
{

SNMPConnector::SNMPConnector(): Connector("SNMP") {}

void SNMPConnector::DoConnect()
{
  if(!read_module(m_MIBFilename.c_str())) { Exception(StatusCode::NOT_FOUND, "Unable to load SNMP MIB file " + m_MIBFilename + "!\n"); }
  struct ::snmp_session session;
  snmp_sess_init(&session);  // structure defaults
  session.version = m_version;
  session.peername = strdup(m_IP.c_str());
  session.community = (u_char*)(strdup(m_readCommunity.c_str()));
  session.community_len = strlen(m_readCommunity.c_str());
  session.timeout = m_timeout;  // timeout (us)
  session.retries = m_retries;  // retries
  if(!(m_session = snmp_open(&session)))
  {
    int   liberr{0};
    int   syserr{0};
    char* errstr{nullptr};
    snmp_error(&session, &liberr, &syserr, &errstr);
    std::string ret{errstr};
    free(errstr);
    throw Exception(StatusCode::FAILURE, ret);
  }
}

void SNMPConnector::DoDisconnect()
{
  snmp_close(m_session);
  m_session = nullptr;
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
    Exception(StatusCode::NOT_FOUND, "\"MIB_path\" key not set !");
  }
  try
  {
    m_IP = toml::find<std::string>(getParameters(), "IP");
  }
  catch(const std::out_of_range& e)
  {
    Exception(StatusCode::NOT_FOUND, "\"IP\" key not set !");
  }
  try
  {
    m_MIBFilename = toml::find<std::string>(getParameters(), "MIB_filename");
  }
  catch(const std::out_of_range& e)
  {
    Exception(StatusCode::NOT_FOUND, "\"MIB_filename\" key not set !");
  }
  m_timeout           = toml::find_or<int>(getParameters(), "Timeout", 300000);
  m_readCommunity     = toml::find_or<std::string>(getParameters(), "ReadCommunity", "public");
  m_retries           = toml::find_or<int>(getParameters(), "Retries", 2);
  std::string version = toml::find_or<std::string>(getParameters(), "Version", "v1");
  if(version != "v1" || version != "v2c" || version != "v3") { Exception(StatusCode::NOT_FOUND, "\"VERSION\" should be v1, v2c or v3 !"); }
  if(version == "v1") m_version = SNMP_VERSION_1;
  else if(version == "v2c") m_version = SNMP_VERSION_2c;
  else m_version = SNMP_VERSION_3;
  SOCK_STARTUP;
  init_snmp("SNMP");
  add_mibdir(m_MIBFilename.c_str());
  init_mib();
}

yaodaq::Response SNMPConnector::send(const yaodaq::Command& command) const
{
  char type{findType(receive(command))};
  netsnmp_pdu* pdu{snmp_pdu_create(SNMP_MSG_SET)};
  netsnmp_pdu* response{nullptr};
  netsnmp_variable_list* vars{nullptr};
  std::size_t name_length{MAX_OID_LEN};
  oid  name[MAX_OID_LEN];
  if(!snmp_parse_oid(command.getKey("method").asCString(),name, &name_length))
  {
    snmp_perror(command.getKey("method").asCString());
    throw -1;
  }
  else if(snmp_add_var(pdu,name,name_length,type,command.getKey("params").asCString()))
  {
    snmp_perror(command.getKey("method").asCString());
    throw -1;
  }
  //do the request
  int status = snmp_synch_response(m_session, pdu, &response);
  if (status == STAT_SUCCESS)
  {
    if(response->errstat == SNMP_ERR_NOERROR)
    {
       for(vars = response->variables; vars;vars = vars->next_variable)print_variable(vars->name, vars->name_length, vars);
    }
    else
    {
      std::cout<<"Error in packet.\nReason: "<<snmp_errstring(response->errstat)<<"\n";
    }
  }
  else if (status == STAT_TIMEOUT)
  {
    std::cout<<"Timeout: No Response from "<<m_IP<<" \n";
  }
  else
  {
    snmp_sess_perror("snmpset",m_session);
  }
  if(response) snmp_free_pdu(response);
  return receive(command);
}

yaodaq::Response SNMPConnector::receive(const yaodaq::Command& command) const
{
  netsnmp_pdu* pdu {snmp_pdu_create(SNMP_MSG_GET)};
  netsnmp_pdu* response{nullptr};
  netsnmp_variable_list* vars{nullptr};
  std::size_t name_length{MAX_OID_LEN};
  oid name[MAX_OID_LEN];
  if(!snmp_parse_oid(command.getKey("method").asCString(),name, &name_length))
  {
    snmp_perror(command.getKey("method").asCString());
    throw -1;
  }
  else snmp_add_null_var(pdu, name, name_length);
  bool stop=false;
  int count=1;
  yaodaq::Response retour;
  do
  {
    int status = snmp_synch_response(m_session, pdu, &response);
    if(status == STAT_SUCCESS)
    {
      if(response->errstat == SNMP_ERR_NOERROR)
      {
        for(vars = response->variables; vars; vars = vars->next_variable)
        {
          if(vars->type == ASN_OCTET_STR)
          {
            bool imstring=true;
            std::string str;
            std::uint64_t toto={0};
            retour.addKey("return type","ASN_OCTET_STR");
            //retour.setPersonalType("ASN_OCTET_STR");
            for(unsigned int i=0;i!=vars->val_len;++i)
            {
              str+=(vars->val.bitstring)[i];
            }
            std::string ret{reinterpret_cast<char*>(vars->val.string)};
            if(ret!=str)
            {
              for(std::size_t i=0;i!=vars->val_len;++i)
              {
                std::bitset<8> p=(vars->val.bitstring)[i];
                for(std::size_t o=0;o!=4;++o)
                {
                  static std::bitset<1> dumb;
                  dumb[0]=p[o];
                  p[o]=p[7-o];
                  p[7-o]=dumb[0];
                }
                toto|=(p.to_ulong()<<8*(i));
              }
              retour.addKey("return",toto);
              //retour=toto;
            }
            else
            {
              for(std::size_t i = 0 ; i < ret.length(); i++)
              {
                if(!iscntrl(ret[i])&&!isprint(ret[i]))
                {
                  imstring=false;
                  break;
                }
              }
              if(imstring==true) retour.addKey("return",str);
              else
              {
                for(std::size_t i=0;i!=vars->val_len;++i)
                {
                  std::bitset<8> p=(vars->val.bitstring)[i];
                  for(std::size_t o=0;o!=4;++o)
                  {
                    static std::bitset<1> dumb;
                    dumb[0]=p[o];
                    p[o]=p[7-o];
                    p[7-o]=dumb[0];
                  }
                  toto|=(p.to_ulong()<<8*(i));
                }
                retour.addKey("return",toto);
                //retour=toto;
              }

            }
          }
          else if(vars->type == ASN_BIT_STR)
          {
            std::string ret{reinterpret_cast<char*>(vars->val.string)};
            ret.erase(vars->val_len);
            retour.addKey("return type","ASN_BIT_STR");
            retour.addKey("return",ret);
          }
          else if(vars->type == ASN_OPAQUE_FLOAT)
          {
            retour.addKey("return type","ASN_OPAQUE_FLOAT");
            retour.addKey("return",((float)*vars->val.floatVal));
          }
          else if(vars->type == ASN_OPAQUE_DOUBLE)
          {
            retour.addKey("return type","ASN_OPAQUE_DOUBLE");
            retour.addKey("return",((double)*vars->val.doubleVal));
          }
          else if(vars->type == ASN_INTEGER)
          {
            retour.addKey("return type","ASN_INTEGER");
            retour.addKey("return",((int)*vars->val.integer));
          }
          else if(vars->type == ASN_IPADDRESS)
          {
            retour.addKey("return type","ASN_IPADDRESS");
            retour.addKey("return",((int)*vars->val.integer));
          }
        }
        stop=true;
      }
      else
      {
        std::cout<<"Error in packet\nReason: "<<snmp_errstring(response->errstat)<<"\n";
      }
    }
    else if(status == STAT_TIMEOUT)
    {
      std::cout<<"Timeout: No Response from "<<m_IP<<" \n";
      stop=true;
    }
    else
    {
      snmp_sess_perror("snmpget",m_session);
      stop=true;
      throw -3;
    }
  }
  while(stop==false);
  if(response!=nullptr) snmp_free_pdu(response);
  return retour;
}

char SNMPConnector::findType(const yaodaq::Response& response) const
{
  if(response.getKey("return type").asString()=="ASN_IPADDRESS") return 'a';
  else if (response.getKey("return type").asString()=="ASN_INTEGER") return 'i';
  else if (response.getKey("return type").asString()=="ASN_OPAQUE_DOUBLE") return 'D';
  else if (response.getKey("return type").asString()=="ASN_OPAQUE_FLOAT") return 'F';
  else if (response.getKey("return type").asString()=="ASN_OCTET_STR") return 's';
  else if (response.getKey("return type").asString()=="ASN_BIT_STR") return 'b';
  else throw Exception(StatusCode::INVALID_PARAMETER,"Invalid parameter.");
}


yaodaq::Response SNMPConnector::sendCommand(const yaodaq::Command& command) const
{
  if(command.getKey("Type")=="Send") return send(command);
  else if(command.getKey("Type")=="Receive") return receive(command);
  else throw Exception(StatusCode::INVALID_PARAMETER,"Type should be Send or Receive.");
}

};  // namespace SNMP
