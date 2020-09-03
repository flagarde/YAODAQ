#pragma once

#include "Connector.hpp"

#include <string>

struct snmp_session;
struct snmp_pdu;
struct session_list;

namespace SNMP
{

class SnmpObject;



class SNMPConnector: public Connector
{
public:
  SNMPConnector();
  virtual ~SNMPConnector() = default;
  virtual void DoDisconnect() final;
  virtual void DoConnect() final;
  Response     sendCommand(const Command& command) const;
protected:

  // Helper functions

  /**
   * @brief Resolves the OID from the textual SNMP description
   * and stores the OID in object.
   * @internal
   * @param node The name to resolve, e.g. "sysMainSwitch"
   * @param object A pointer to SnmpObject to store the resolved OID
   * @return true on success, false otherwise
   */
  int getNode(const char * const node, SnmpObject *object);

  /**
   * @brief Resolves the OID from the textual SNMP description
   * with appended index and stores the OID in object.
   * @internal
   * @param nodeBase The node base name, e.g. "outputSwitch"
   * @param index The node index, e.g. 100 for "outputSwitch.100"
   * @param object A pointer to SnmpObject to store the resolved OID
   * @return true on success, false otherwise
   */
  int getIndexNode(const char * const nodeBase, int index, SnmpObject *object);

  double snmpSetDouble(const SnmpObject &object,const double& value);
  double snmpGetDouble(const SnmpObject &object);
  ::snmp_pdu* prepareGetRequestPdu(const SnmpObject *objects,const std::size_t& size);
  void updateSnmpErrorCode(int status, struct snmp_pdu *response);
  void logErrors(struct snmp_pdu *response,
                       const SnmpObject &object, int status, const char *functionName);

  ::snmp_pdu *prepareSetRequestPdu();
  virtual void    verifyParameters() final;
  ::snmp_session* m_session{nullptr};
  ::session_list* m_session_list{nullptr};
  std::string     m_MIBFilename{""};
  std::string     m_MIBPath{""};
  std::string     m_IP{""};
  std::string     m_readCommunity{"public"};
  long            m_timeout{300000};  // timeout (us)
  int             m_retries{2};       // retries
  int             m_version{1};
  int  m_errorCode{0};
};




}  // namespace SNMP
