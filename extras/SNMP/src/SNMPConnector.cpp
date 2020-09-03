#include "SNMPConnector.hpp"

#define WIN32_LEAN_AND_MEAN
#include "Exception.hpp"
#include "StatusCode.hpp"
#include "net-snmp-config.h"
#include "net-snmp-includes.h"

#include <memory>

namespace SNMP
{

/**
* @brief The SnmpObject class is used internally to resolve OIDs and for the SNMP calls.
*/
class SnmpObject
{
public:
  ::oid    id[MAX_OID_LEN]; ///< The resolved SNMP OID
  std::size_t len;             ///< The resolved OIDs length in byte
  char   desc[100];       ///< The OIDs textual representation, e.g. "sysDescr.0"
};

// Helper functions

/**
 * @brief Resolves the OID from the textual SNMP description
 * and stores the OID in object.
 * @internal
 * @param node The name to resolve, e.g. "sysMainSwitch"
 * @param object A pointer to SnmpObject to store the resolved OID
 * @return true on success, false otherwise
 */
int SNMPConnector::getNode(const char * const node, SnmpObject *object)
{
  object->len = MAX_OID_LEN;
  if (!get_node(node, object->id, &object->len)) {
    snmp_log(LOG_ERR, "OID %s not found!\n", node);
    return false;
  }

  snmpStringCopy(object->desc, sizeof(object->desc), node);

  return true;
}

/**
 * @brief Resolves the OID from the textual SNMP description
 * with appended index and stores the OID in object.
 * @internal
 * @param nodeBase The node base name, e.g. "outputSwitch"
 * @param index The node index, e.g. 100 for "outputSwitch.100"
 * @param object A pointer to SnmpObject to store the resolved OID
 * @return true on success, false otherwise
 */
int SNMPConnector::getIndexNode(const char * const nodeBase, int index, SnmpObject *object)
{
  char node[100];

  strPrintf(node, sizeof(node), "%s.%i", nodeBase, index);

  return getNode(node, object);
}



::snmp_pdu* SNMPConnector::prepareGetRequestPdu(const SnmpObject *objects,const std::size_t& size)
{
  struct snmp_pdu *pdu = snmp_pdu_create(SNMP_MSG_GET);
  for(std::size_t i = 0; i < size; ++i) snmp_add_null_var(pdu, objects[i].id, objects[i].len); // generate request data
  return pdu;
}


void SNMPConnector::logErrors(struct snmp_pdu *response,
               const SnmpObject &object, int status, const char *functionName)
{
  // FAILURE: print what went wrong!
  if (status == STAT_SUCCESS)
    snmp_log(LOG_ERR, "%s(%s): Error in packet. Reason: %s\n",
             functionName, object.desc, snmp_errstring(response->errstat));
    else
      snmp_sess_perror("snmpget", snmp_sess_session(m_session_list));
}

void SNMPConnector::updateSnmpErrorCode(int status, struct snmp_pdu *response)
{
  if ( (status == STAT_ERROR) || (response == NULL) )
  {
    m_errorCode = STAT_ERROR;
  }
  else if (status == STAT_TIMEOUT)
  {
    m_errorCode = SNMPERR_TIMEOUT;
  }
  else if (response->errstat)
  {
    m_errorCode = response->errstat;
  }
  else if (
    response->variables->type == SNMP_NOSUCHINSTANCE ||
    response->variables->type == SNMP_NOSUCHOBJECT ||
    response->variables->type == SNMP_ENDOFMIBVIEW
  )
  {
    m_errorCode = response->variables->type;
  }
  else
  {
    m_errorCode = SNMP_ERR_NOERROR;
  }
}

double getDoubleVariable(struct variable_list *vars, int *errorCode = 0)
{
  if (errorCode)
    *errorCode = SNMP_ERR_NOERROR;

  if (vars->type == ASN_OPAQUE_FLOAT)
    return *vars->val.floatVal;
  else if (vars->type == ASN_OPAQUE_DOUBLE)
    return *vars->val.doubleVal;
  else if (vars->type == ASN_INTEGER)
    return (double)*vars->val.integer;

  if (errorCode)
    *errorCode = vars->type;
  return 0.0;
}

::snmp_pdu* SNMPConnector::prepareSetRequestPdu()
{
  struct snmp_pdu *pdu = snmp_pdu_create(SNMP_MSG_SET);
  pdu->community    = (u_char*)strdup(m_readCommunity.c_str());
  pdu->community_len = strlen(m_readCommunity.c_str());
  return pdu;
}


double SNMPConnector::snmpGetDouble(const SnmpObject &object)
{
  double value{0.0};
  struct snmp_pdu *pdu = prepareGetRequestPdu(&object, 1);

  struct snmp_pdu *response;
  int status = snmp_sess_synch_response(m_session_list, pdu, &response);
  updateSnmpErrorCode(status, response);

  if (status == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR) {
    value = getDoubleVariable(response->variables);
  } else {
    logErrors(response, object, status, "snmpGetDouble");
  }

  snmp_free_pdu(response);
  return value;
}

double SNMPConnector::snmpSetDouble(const SnmpObject &object,const double& value)
{
  struct snmp_pdu *pdu = prepareSetRequestPdu();

  float v = (float)value;
  snmp_pdu_add_variable(pdu, object.id, object.len, ASN_OPAQUE_FLOAT, (u_char *)&v, sizeof(v));

  double result = 0.0;

  struct snmp_pdu *response;
  int status = snmp_sess_synch_response(m_session_list, pdu, &response);
  updateSnmpErrorCode(status, response);

  if (status == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR) {
    result = getDoubleVariable(response->variables);
  } else {
    logErrors(response, object, status, "snmpSetDouble");
  }

  snmp_free_pdu(response);
  return result;
}

/*char *snmpGetString(HSNMP session, const SnmpObject &object)
{
  struct snmp_pdu *pdu = prepareGetRequestPdu(&object, 1);

  struct snmp_pdu *response;
  int status = snmp_sess_synch_response(session, pdu, &response);
  updateSnmpErrorCode(status, response);

  memset(m_stringBuffer, 0, sizeof(m_stringBuffer));

  if (status == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR) {
    struct variable_list *vars = response->variables;
    if (vars->type == ASN_OCTET_STR) {
      size_t len = sizeof(m_stringBuffer) - 1;
      if (len > vars->val_len)
        len = vars->val_len;

      memcpy(m_stringBuffer, vars->val.string, len);
      m_stringBuffer[len] = 0;
    }
  } else {
    logErrors(session, response, object, status, "snmpGetString");
  }

  snmp_free_pdu(response);
  return m_stringBuffer;
}*/


SNMPConnector::SNMPConnector(): Connector("SNMP") {}


void SNMPConnector::DoConnect()
{
  SOCK_STARTUP;
  struct ::snmp_session session;
  snmp_sess_init(&session);  // structure defaults
  session.version       = m_version;
  session.peername      = strdup(m_IP.c_str());
  session.community     = (u_char*)strdup(m_readCommunity.c_str());
  session.community_len = strlen(m_readCommunity.c_str());
  session.timeout       = m_timeout;  // timeout (us)
  session.retries       = m_retries;  // retries
  if(!(m_session = snmp_open(&session)))
  {
    int   liberr{0};
    int   syserr{0};
    char* errstr{nullptr};
    snmp_error(&session, &liberr, &syserr, &errstr);
    std::string ret = errstr;
    free(errstr);
    throw Exception(StatusCode::FAILURE, ret);
  }
  m_session_list = snmp_sess_pointer(m_session);
}

void SNMPConnector::DoDisconnect()
{
  snmp_close(m_session);
  m_session = nullptr;
  m_session_list =nullptr;
  SOCK_CLEANUP;
}

void SNMPConnector::verifyParameters()
{
  try
  {
    m_MIBPath = toml::find<std::string>(getParameters(),"MIB_path");
  }
  catch(const std::out_of_range& e)
  {
    Exception(StatusCode::NOT_FOUND, "\"MIB_path\" key not set !");
  }
  try
  {
    m_IP = toml::find<std::string>(getParameters(),"IP");
  }
  catch(const std::out_of_range& e)
  {
    Exception(StatusCode::NOT_FOUND, "\"IP\" key not set !");
  }
  try
  {
    m_MIBFilename = toml::find<std::string>(getParameters(),"MIB_filename");
  }
  catch(const std::out_of_range& e)
  {
    Exception(StatusCode::NOT_FOUND, "\"MIB_filename\" key not set !");
  }
  m_timeout           = toml::find_or<int>(getParameters(), "Timeout", 300000);
  m_readCommunity     = toml::find_or<std::string>(getParameters(), "ReadCommunity", "public");
  m_retries           = toml::find_or<int>(getParameters(), "Retries", 2);
  std::string version = toml::find_or<std::string>(getParameters(), "Version", "v1");
  if(version != "v1" || version != "v2c" || version != "v3")
  {
    Exception(StatusCode::NOT_FOUND, "\"VERSION\" should be v1, v2c or v3 !");
  }
  if(version == "v1") m_version = SNMP_VERSION_1;
  else if(version == "v2c") m_version = SNMP_VERSION_2c;
  else m_version = SNMP_VERSION_3;
  init_snmp("");
  add_mibdir(m_MIBPath.c_str());
  init_mib();
  if(!read_module(m_MIBFilename.c_str()))
  {
    Exception(StatusCode::NOT_FOUND, "Unable to load SNMP MIB file " + m_MIBFilename + "!\n");
  }
  SOCK_STARTUP;
}

Response SNMPConnector::sendCommand(const Command& command) const
{
  if(command.getParameter("Direction") == "ERROR") throw Exception(StatusCode::NOT_FOUND, "No key Direction SEND or RECEIVE found in the command !");
  else if(command.getParameter("Direction") == "SEND")
  {
    //return sendInfos(Command);
  }
  else if(command.getParameter("Direction") == "RECEIVE")
  {
    //return receiveInfos(Command);
  }
  else
    throw Exception(StatusCode::INVALID_PARAMETER, "Key Direction must contain SEND or RECEIVE !");
}

}  // namespace SNMP
