#pragma once

#include "Board.hpp"

#include <cstdint>
namespace CAEN
{
class CAENCommBoard: public Board
{
public:
  CAENCommBoard(const std::string& name);

  /*##########################################################################*/
  /*
  ** COMMUNICATION LAYER
  */
  /*##########################################################################*/

  /**************************************************************************/ /**
                                                                                * \fn      void Write32(const uint32_t& Address,const uint32_t& Data)
                                                                                * \brief   Write a 32 bit register of the device
                                                                                *
                                                                                * \param   [IN]  Address: register address offset
                                                                                * \param   [IN]  Data: new register content to write into the device
                                                                                ******************************************************************************/
  void Write32(const std::uint32_t& Address, const std::uint32_t& Data);

  /**************************************************************************/ /**
                                                                                * \fn      void Write16(const uint32_t& Address,const uint16_t& Data)
                                                                                * \brief   Write a 16 bit register of the device
                                                                                *
                                                                                * \param   [IN]  Address: register address offset
                                                                                * \param   [IN]  Data: new register content to write into the device
                                                                                ******************************************************************************/
  void Write16(const std::uint32_t& Address, const std::uint16_t& Data);

  /**************************************************************************/ /**
                                                                                * \fn      uint32_t CAENComm_Read32(uint32_t Address)
                                                                                * \brief   Read a 32 bit data from the specified register.
                                                                                *
                                                                                * \param   [IN]  Address: register address offset
                                                                                * \return  Data: The data read from the device
                                                                                ******************************************************************************/
  std::uint32_t Read32(const std::uint32_t& Address);

  /**************************************************************************/ /**
                                                                                * \fn      uint16_t Read16(uint31_t Address)
                                                                                * \brief   Read a 16 bit data from the specified register.
                                                                                *
                                                                                * \param   [IN]  Address: register address offset
                                                                                * \return  Data: The data read from the device
                                                                                ******************************************************************************/
  std::uint16_t Read16(const std::uint32_t& Address);

  /**************************************************************************/ /**
                                                                                * \fn     std::vector<uint32_t> MultiRead32(const uint32_t Address,const int& nCycles)
                                                                                * \brief   The function performs a block of single 32bit ReadRegister.
                                                                                *
                                                                                * \param   [IN]  Address: register address offsets
                                                                                * \param   [IN]  nCycles: the number of read to perform
                                                                                * \return  std::vector<uint32_t>: The datas read from the device
                                                                                ******************************************************************************/
  std::vector<std::uint32_t> MultiRead32(std::uint32_t& Address,
                                         const int&     nCycles);

  /**************************************************************************/ /**
                                                                                * \fn      std::vector<uint16_t> MultiRead16(const uint32_t Address,const int& nCycles)
                                                                                * \brief   The function performs a block of single 16bit ReadRegister.
                                                                                *
                                                                                * \param   [IN]  Address: register address offsets
                                                                                * \param   [IN]  nCycles: the number of read to perform
                                                                                * \return std::vector<uint16_t>: The datas read from the device
                                                                                ******************************************************************************/
  std::vector<std::uint16_t> MultiRead16(std::uint32_t& Address,
                                         const int&     nCycles);

  /**************************************************************************/ /**
                                                                                * \fn      void MultiWrite16(const std::uint32_t& Address,const int& nCycles,std::vector<std::uint16_t>& data)
                                                                                * \brief   The function performs a block of single 16bit WriteRegister.
                                                                                *
                                                                                * \param   [IN]  Address: register address offsets
                                                                                * \param   [IN]  nCycles: the number of read to perform
                                                                                * \param   [IN]  data: The datas to write to the device
                                                                                ******************************************************************************/
  void MultiWrite16(std::uint32_t& Address, const int& nCycles,
                    std::vector<std::uint16_t>& data);

  /**************************************************************************/ /**
                                                                                * \fn      void MultiWrite32(const std::uint32_t& Address,const int& nCycles,std::vector<std::uint32_t>& data)
                                                                                * \brief   The function performs a block of single 16bit WriteRegister.
                                                                                *
                                                                                * \param   [IN]  Address: register address offsets
                                                                                * \param   [IN]  nCycles: the number of read to perform
                                                                                * \param   [IN]  data: The datas to write to the device
                                                                                ******************************************************************************/
  void MultiWrite32(std::uint32_t& Address, const int& nCycles,
                    std::vector<std::uint32_t>& data);

  /**************************************************************************/ /**
                                                                                * \fn      CAENComm_ErrorCode STDCALL CAENComm_BLTRead(int handle, uint32_t Address, uint32_t *Buff, int BltSize, int *nw)
                                                                                * \brief   Read a block of data from the device using a BLT (32 bit) cycle
                                                                                *
                                                                                * \param   [IN]  Address: data space starting address
                                                                                * \param   [IN]  BltSize: size of the Block Read Cycle (in bytes)
                                                                                * \param   [OUT] buff: pointer to the data buffer
                                                                                * \param   [OUT] nw: number of longwords (32 bit) actually read from the device
                                                                                * \return  0 = Success; negative numbers are error codes
                                                                                ******************************************************************************/
  // CAENComm_ErrorCode STDCALL CAENComm_BLTRead(int handle, uint32_t Address,
  // uint32_t *Buff, int BltSize, int *nw);

  /**************************************************************************/ /**
                                                                                * \fn      CAENComm_ErrorCode STDCALL CAENComm_MBLTRead(int handle, uint32_t Address, uint32_t *Buff, int BltSize, int *nw)
                                                                                * \brief   Read a block of data from the device using an MBLT (64 bit) cycle
                                                                                *
                                                                                * \param   [IN]  Address: data space starting address
                                                                                * \param   [IN]  BltSize: size of the Block Read Cycle (in bytes)
                                                                                * \param   [OUT] buff: pointer to the data buffer
                                                                                * \param   [OUT] nw: number of longwords (32 bit) actually read from the device
                                                                                * \return  0 = Success; negative numbers are error codes
                                                                                ******************************************************************************/
  // CAENComm_ErrorCode STDCALL CAENComm_MBLTRead(int handle, uint32_t Address,
  // uint32_t *Buff, int BltSize, int *nw);

  /**************************************************************************/ /**
   * \fn      CAENComm_ErrorCode STDCALL CAENComm_VMEIRQCheck(int handle,
   uint8_t *Mask)
   * \brief   The function returns a bit mask indicating the active VME IRQ
   lines.

   * \param   [IN]  handle: device handler
   * \param   [OUT]  Mask: A bit-mask indicating the VME IRQ lines.
   * \return  0 = Success; negative numbers are error codes
   ******************************************************************************/
  // CAENComm_ErrorCode STDCALL CAENComm_VMEIRQCheck(int VMEhandle, uint8_t
  // *Mask);

  /**************************************************************************/ /**
                                                                                * \fn      void IRQDisable()
                                                                                * \brief   The function disables the IRQ lines.
                                                                                ******************************************************************************/
  void IRQDisable();

  /**************************************************************************/ /**
                                                                                * \fn      void IRQEnable()
                                                                                * \brief   The function enaables the IRQ lines.
                                                                                ******************************************************************************/
  void IRQEnable();

  /**************************************************************************/ /**
   * \fn      CAENComm_ErrorCode STDCALL CAENComm_VMEIACKCycle16(int VMEhandle,
   IRQLevels Level, int *BoardID)
   * \brief   The function performs a 16 bit VME interrupt acknowledge cycle .

   * \param   [IN]  Level: The VME IRQ level to acknowledge (see IRQLevels
   enum).
   * \param   [OUT] BoardID: the Id of the Board that reased the interrupt.
   * \return  0 = Success; negative numbers are error codes
   ******************************************************************************/
  // CAENComm_ErrorCode STDCALL CAENComm_VMEIACKCycle16(int VMEhandle, IRQLevels
  // Level, int *BoardID);

  /**************************************************************************/ /**
   * \fn      CAENComm_ErrorCode STDCALL CAENComm_VMEIACKCycle32(int VMEhandle,
   IRQLevels Level, int *BoardID)
   * \brief   The function performs a 32 bit VME interrupt acknowledge cycle.

   * \param   [IN]  Level: The VME IRQ level to acknowledge (see IRQLevels
   enum).
   * \param   [OUT] BoardID: the Id of the Board that reased the interrupt.
   * \return  0 = Success; negative numbers are error codes
   ******************************************************************************/
  // CAENComm_ErrorCode STDCALL CAENComm_VMEIACKCycle32(int VMEhandle, IRQLevels
  // Level, int *BoardID);

  /**************************************************************************/ /**
   * \fn      CAENComm_ErrorCode STDCALL CAENComm_IACKCycle(int handle,
   IRQLevels Level, int *BoardID)
   * \brief   The function performs a 16 bit VME interrupt acknowledge cycle .

   * \param   [IN]  Level: The VME IRQ level to acknowledge (see IRQLevels
   enum).
   * \param   [OUT] BoardID: the Id of the Board that reased the interrupt.
   * \return  0 = Success; negative numbers are error codes
   ******************************************************************************/
  // CAENComm_ErrorCode STDCALL CAENComm_IACKCycle(int handle, IRQLevels Level,
  // int *BoardID);

  /**************************************************************************/ /**
   * \fn      IRQWait(const uint32_t& Timeout)
   * \brief   The function wait the IRQ until one of them raise or timeout
   expires. This function can be used ONLY on board NOT controlled by CAEN VME
   Bridges
   * \param   [IN]  Timeout: Timeout in milliseconds.
   ******************************************************************************/
  void IRQWait(const std::uint32_t& Timeout);

  /**************************************************************************/ /**
   * \fn      CAENComm_ErrorCode STDCALL CAENComm_VMEIRQWait(int LinkType, int
   LinkNum, int ConetNode, uint8_t IRQMask, uint32_t Timeout, int *VMEHandle)
   * \brief   The function wait the IRQ until one of them raise or timeout
   expires.

   * \param   [IN] LinkType: the link used to connect to the CAEN VME
   Bridge(CAENComm_USB|CAENComm_PCI_OpticalLink|CAENComm_PCIE_OpticalLink|CAENComm_PCIE)
   * \param   [IN] LinkNum: The link number
   * \param   [IN] ConetNode: The CAEN VME Bridge number in the link.
   * \param   [IN] Timeout: Timeout in milliseconds.
   * \param   [IN] IRQMask: A bit-mask indicating the IRQ lines
   * \param   [OUT] VMEHandle: The CAEN Bridhe handle to use in VMEIRQCheck and
   VMEIACKCycle
   * \return  0 = Success; negative numbers are error codes
   ******************************************************************************/
  // CAENComm_ErrorCode STDCALL CAENComm_VMEIRQWait(CAENComm_ConnectionType
  // LinkType, int LinkNum, int ConetNode, uint8_t IRQMask, uint32_t Timeout,
  // int *VMEHandle);

  std::uint32_t GetVMEHandler();

  std::string GetPCIBoardSerialNumber();

  std::string GetPCIFirmwareRelease();

  std::string GetVMESerialNumber();

  std::string GetVMEFirmwareRelease();

  std::string GetVMEOpticalChipsetFirmwareRelease();

  /**************************************************************************/ /**
                                                                                * \fn      std::string SWRelease()
                                                                                * \brief   Returns the Software Release of the library
                                                                                *
                                                                                * \return  SwRel: the Software Release of the library
                                                                                ******************************************************************************/
  std::string SWRelease();
};

}  // namespace CAEN
