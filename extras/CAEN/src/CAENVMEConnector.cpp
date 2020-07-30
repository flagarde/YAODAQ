#include "CAENVMEConnector.hpp"

#include "CAENVMEException.hpp"
#include "CAENVMElib.h"
#include "StatusCode.hpp"
#include "magic_enum.hpp"
#include "spdlog.h"
#include "toml.hpp"

namespace CAEN
{
enum class VMEBridgeModel
{
  V1718,
  V2718,
  A2818,
  A2719,
  A3818
};

CAENVMEConnector::CAENVMEConnector(): Connector("CAENVME") {}

void CAENVMEConnector::DoConnect()
{
  CAENVMEException(CAENVME_Init(static_cast<CVBoardTypes>(magic_enum::enum_cast<VMEBridgeModel>(m_Model).value()), m_LinkNumber, m_ConetNode, &m_Handle));
}

void CAENVMEConnector::DoDisconnect()
{
  CAENVMEException(CAENVME_End(m_Handle));
}

void CAENVMEConnector::verifyParameters()
{
  printParameters();
  try
  {
    m_Model = toml::find_or<std::string>(getParameters(), "Model", "");
  }
  catch(...)
  {
    throw Exception(StatusCode::NOT_FOUND, "key \"Model\" not set !");
  }
  auto bridge = magic_enum::enum_cast<VMEBridgeModel>(m_Model);
  if(!bridge.has_value()) throw Exception(StatusCode::INVALID_PARAMETER, "Model \"" + m_Model + "\" Unknown ! \n Only V1718, V2718, A2818, A2719 and A3818 are implemented !");
  try
  {
    m_LinkNumber = toml::find<short>(getParameters(), "Link Number");
  }
  catch(...)
  {
    throw Exception(StatusCode::NOT_FOUND, "key \"Link Number\" not set !");
  }
  if(m_Model == "A2818" || m_Model == "A2719" || m_Model == "A3818")
  {
    try
    {
      m_ConetNode = toml::find<short>(getParameters(), "Conet Node");
    }
    catch(...)
    {
      throw Exception(StatusCode::NOT_FOUND, "key \"Board Number\" not set !");
    }
  }
}

}  // namespace CAEN
