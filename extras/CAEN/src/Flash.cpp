#include "Flash.hpp"

#include "CAENFlashException.hpp"

namespace CAEN
{
uint32_t Flash::get_spi_address(const uint16_t& page_addr, const uint16_t& byte_addr)
{
  switch(m_DeviceID)
  {
    case m_SPIFLASH_DEVICEID_64MBIT:
      return ((page_addr & 0x1FFF) << 11) | (byte_addr & 0x7FF);  // PA[12:0] & BA[10:0]
    case m_SPIFLASH_DEVICEID_32MBIT:
      return ((page_addr & 0x1FFF) << 10) | (byte_addr & 0x3FF);  // PA[12:0] & BA[9:0]
    case m_SPIFLASH_DEVICEID_8MBIT:
      return ((page_addr & 0x0FFF) << 9) | (byte_addr & 0x1FF);  // PA[11:0] & BA[8:0]
    default:
      return 0;
  }
}

std::vector<uint8_t> Flash::read_bytes(const uint32_t& addr, const uint16_t& len)
{
  if(!m_IsInitialized) CAENFlashException((int)UNINITIALIZED);
  uint32_t page_addr{addr / m_PAGE_SIZE};
  uint32_t byte_addr{addr % m_PAGE_SIZE};
  uint32_t spi_address = get_spi_address((uint16_t)page_addr, (uint16_t)byte_addr);
  m_SPI.select();
  m_SPI.write(m_CONTINOUS_ARRAY_READ_LOW_FREQ);
  /* address */
  m_SPI.write(((spi_address >> 16) & 0xFF));
  m_SPI.write(((spi_address >> 8) & 0xFF));
  m_SPI.write(((spi_address >> 0) & 0xFF));
  std::vector<uint8_t> ret = m_SPI.read_block(len);
  m_SPI.unselect();
  return ret;
}

std::vector<uint8_t> Flash::read_virtual_page(const uint16_t& page)
{
  if(!m_IsInitialized) CAENFlashException((int)UNINITIALIZED);
  uint16_t standard_page_addr{page * m_VIRTUAL_PAGE_SIZE};
  uint16_t byte_addr{0};
  return read_bytes((uint32_t)(standard_page_addr + byte_addr), m_VIRTUAL_PAGE_SIZE);
}

void Flash::setHandle(const std::uint32_t& han)
{
  m_SPI.setHandle(han);
  m_Handle = han;
}

void Flash::read_device_id()
{
  std::uint8_t data{0};
  m_SPI.select();
  m_SPI.write(m_READ_MANUFACTURER_DEVICE_ID);
  data       = m_SPI.read(); /* Skip manufacturer */
  data       = m_SPI.read();
  m_DeviceID = data & 0xFF;
  m_SPI.unselect();
}

void Flash::init()
{
  read_device_id();
  switch(m_DeviceID)
  {
    case m_SPIFLASH_DEVICEID_64MBIT:
      m_PAGE_SIZE     = 1056;
      m_IsInitialized = true;
      break;
    case m_SPIFLASH_DEVICEID_32MBIT:
      m_PAGE_SIZE     = 528;
      m_IsInitialized = true;
      break;
    case m_SPIFLASH_DEVICEID_8MBIT:
      m_PAGE_SIZE     = 264;
      m_IsInitialized = true;
      break;
    default:
      m_IsInitialized = false;
      CAENFlashException((int)UNSUPPORTED_FLASH_DEVICE);
      break;
  }
}

}  // namespace CAEN
