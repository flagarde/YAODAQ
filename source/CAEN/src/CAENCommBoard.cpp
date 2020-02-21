#include "CAENCommBoard.hpp"

#include "CAENComm.h"
#include "CAENCommError.hpp"

#include <vector>

namespace CAEN
{
CAENCommBoard::CAENCommBoard(const std::string& name)
    : Board(name, "CAENCommBoard")
{
}

void CAENCommBoard::Write32(const std::uint32_t& Address,
                            const std::uint32_t& Data)
{
  CAENCommError(CAENComm_Write32(m_Handle, Address, Data));
}

void CAENCommBoard::Write16(const std::uint32_t& Address,
                            const std::uint16_t& Data)
{
  CAENCommError(CAENComm_Write16(m_Handle, Address, Data));
}

std::uint32_t CAENCommBoard::Read32(const std::uint32_t& Address)
{
  std::uint32_t ret{0};
  CAENCommError(CAENComm_Read32(m_Handle, Address, &ret));
  return ret;
}

std::uint16_t CAENCommBoard::Read16(const std::uint32_t& Address)
{
  std::uint16_t ret{0};
  CAENCommError(CAENComm_Read16(m_Handle, Address, &ret));
  return ret;
}

std::vector<std::uint32_t> CAENCommBoard::MultiRead32(std::uint32_t& Address,
                                                      const int&     nCycles)
{
  CAENComm_ErrorCode         erros[nCycles];
  std::vector<std::uint32_t> ret;
  ret.reserve(nCycles);
  CAENCommError(
      CAENComm_MultiRead32(m_Handle, &Address, nCycles, &ret[0], erros));
  for(std::size_t i = 0; i != nCycles; ++i)
  { CAENCommError(static_cast<int>(erros[i])); }
  return ret;
}

std::vector<std::uint16_t> CAENCommBoard::MultiRead16(std::uint32_t& Address,
                                                      const int&     nCycles)
{
  CAENComm_ErrorCode         erros[nCycles];
  std::vector<std::uint16_t> ret;
  ret.reserve(nCycles);
  CAENCommError(
      CAENComm_MultiRead16(m_Handle, &Address, nCycles, &ret[0], erros));
  for(std::size_t i = 0; i != nCycles; ++i)
  { CAENCommError(static_cast<int>(erros[i])); }
  return ret;
}

void CAENCommBoard::MultiWrite16(std::uint32_t& Address, const int& nCycles,
                                 std::vector<std::uint16_t>& data)
{
  CAENComm_ErrorCode erros[nCycles];
  CAENCommError(
      CAENComm_MultiWrite16(m_Handle, &Address, nCycles, &data[0], erros));
  for(std::size_t i = 0; i != nCycles; ++i)
  { CAENCommError(static_cast<int>(erros[i])); }
}

void CAENCommBoard::MultiWrite32(std::uint32_t& Address, const int& nCycles,
                                 std::vector<std::uint32_t>& data)
{
  CAENComm_ErrorCode erros[nCycles];
  CAENCommError(
      CAENComm_MultiWrite32(m_Handle, &Address, nCycles, &data[0], erros));
  for(std::size_t i = 0; i != nCycles; ++i)
  { CAENCommError(static_cast<int>(erros[i])); }
}

/*
CAENComm_ErrorCode STDCALL CAENComm_BLTRead(int handle, uint32_t Address,
uint32_t *Buff, int BltSize, int *nw);

CAENComm_ErrorCode STDCALL CAENComm_MBLTRead(int handle, uint32_t Address,
uint32_t *Buff, int BltSize, int *nw);
*/

// CAENComm_ErrorCode STDCALL CAENComm_VMEIRQCheck(int VMEhandle, uint8_t
// *Mask);

void CAENCommBoard::IRQDisable()
{
  CAENCommError(CAENComm_IRQDisable(m_Handle));
}

void CAENCommBoard::IRQEnable()
{
  CAENCommError(CAENComm_IRQEnable(m_Handle));
}

/*

CAENComm_ErrorCode STDCALL CAENComm_VMEIACKCycle16(int VMEhandle, IRQLevels
Level, int *BoardID);

CAENComm_ErrorCode STDCALL CAENComm_VMEIACKCycle32(int VMEhandle, IRQLevels
Level, int *BoardID);

CAENComm_ErrorCode STDCALL CAENComm_IACKCycle(int handle, IRQLevels Level, int
*BoardID);
*/

void CAENCommBoard::IRQWait(const uint32_t& Timeout)
{
  CAENCommError(CAENComm_IRQWait(m_Handle, Timeout));
}

/*
CAENComm_ErrorCode STDCALL CAENComm_VMEIRQWait(CAENComm_ConnectionType LinkType,
int LinkNum, int ConetNode, uint8_t IRQMask, uint32_t Timeout, int *VMEHandle);
*/

std::string CAENCommBoard::SWRelease()
{
  char ret[10];
  CAENCommError(CAENComm_SWRelease(ret));
  return ret;
}

std::uint32_t CAENCommBoard::GetVMEHandler()
{
  std::uint32_t ret{0};
  CAENCommError(CAENComm_Info(m_Handle, CAENComm_VMELIB_handle, &ret));
  return ret;
}

std::string CAENCommBoard::GetPCIBoardSerialNumber()
{
  char ret[10];
  CAENCommError(CAENComm_Info(m_Handle, CAENComm_PCI_Board_SN, &ret));
  return ret;
}

std::string CAENCommBoard::GetPCIFirmwareRelease()
{
  char ret[10];
  CAENCommError(CAENComm_Info(m_Handle, CAENComm_PCI_Board_FwRel, &ret));
  return ret;
}

std::string CAENCommBoard::GetVMESerialNumber()
{
  char ret[10];
  CAENCommError(CAENComm_Info(m_Handle, CAENComm_VME_Bridge_SN, &ret));
  return ret;
}

std::string CAENCommBoard::GetVMEFirmwareRelease()
{
  char ret[10];
  CAENCommError(CAENComm_Info(m_Handle, CAENComm_VME_Bridge_FwRel1, &ret));
  return ret;
}

std::string CAENCommBoard::GetVMEOpticalChipsetFirmwareRelease()
{
  char ret[10];
  CAENCommError(CAENComm_Info(m_Handle, CAENComm_VME_Bridge_FwRel2, &ret));
  return ret;
}

}  // namespace CAEN
