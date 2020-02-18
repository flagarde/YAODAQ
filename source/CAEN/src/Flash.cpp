#include "Flash.hpp"
#include "CAENFlashError.hpp"

namespace CAEN
{
  
enum FLASH_API_ERROR_CODES 
{
  FLASH_API_SUCCESS = 0,
  FLASH_API_ACCESS_FAILED = -1,
  FLASH_API_PARAMETER_NOT_ALLOWED = -2,
  FLASH_API_FILE_OPEN_ERROR = -3,
  FLASH_API_MALLOC_ERROR = -4,
  FLASH_API_UNINITIALIZED = -5,
  FLASH_API_NOT_IMPLEMENTED = -6,
  FLASH_API_UNSUPPORTED_FLASH_DEVICE = -7
};



void Flash::setHandle(const std::uint32_t& han)
{
  m_SPI.setHandle(han);
  m_Handle=han;
}

void Flash::read_device_id() 
{
  std::uint8_t data{0};
  m_SPI.select();
  m_SPI.write(m_READ_MANUFACTURER_DEVICE_ID);
  data=m_SPI.read(); /* Skip manufacturer */
  data=m_SPI.read();
  m_DeviceID = data & 0xFF;
  m_SPI.unselect();
}

void Flash::init() 
{
  read_device_id();
  switch (m_DeviceID) 
  {
    case m_SPIFLASH_DEVICEID_64MBIT:
      m_PAGE_SIZE = 1056;
      m_IsInitialized = true;
      break;
    case m_SPIFLASH_DEVICEID_32MBIT:
      m_PAGE_SIZE = 528;
      m_IsInitialized = true;
      break;
    case m_SPIFLASH_DEVICEID_8MBIT:
      m_PAGE_SIZE = 264;
      m_IsInitialized = true;
      break;
    default:
      m_IsInitialized = false;
      CAENFlashError((int)FLASH_API_UNSUPPORTED_FLASH_DEVICE);
      break;
  }
}


}
