#ifndef FLASH_HPP
#define FLASH_HPP
#include "CAENFlashError.hpp"
#include "SPI.hpp"

namespace CAEN
{

class Flash 
{
public:
  void setHandle(const std::uint32_t& han);
  /* Main functions to access flash with virtual page size (VIRTUAL_PAGE_SIZE) */
  /* handle is a CAENComm library handle */
  void init();
 /* read_virtual_page(uint16_t page, uint8_t *buf);
  read_virtual_page_ex(uint16_t page,uint16_t offset, uint32_t size,uint8_t *buf);
  write_virtual_page(uint16_t page, uint8_t *buf);
  write_virtual_page_ex(uint16_t page,uint16_t offset, uint32_t size,uint8_t *buf);*/

  /* Flash identification functions */
/*  get_page_size(int *size);
  read_id(uint8_t *manufacturerId,uint8_t *deviceId);
  read_manufacturer_id(uint8_t *manufacturerId);*/
  void read_device_id();
 /* read_unique_id(uint8_t *UniqueId);*/

  /* content access */
 /* read_byte(uint32_t addr, uint8_t *data);
  read_bytes(uint32_t addr, uint8_t *buf,uint16_t len);
  write_byte(uint32_t addr, uint8_t data);
  write_bytes(uint32_t addr, uint8_t *buf,uint16_t len);*/

  /* Buffer oriented operations */
 /* read_buffer1(int buf_addr, uint16_t len,uint8_t *buf);
  read_buffer2(int buf_addr, uint16_t len,uint8_t *buf);
  write_buffer1(uint32_t buf_addr, uint8_t *buf,uint16_t len);
  write_buffer2(uint32_t buf_addr, uint8_t *buf,uint16_t len);*/

  /* Page oriented operations */
 /* read_page(uint16_t page, uint8_t *buf);
  read_page_into_buffer1(uint16_t page);
  read_page_into_buffer2(uint16_t page);
  write_page(uint16_t page, uint8_t *buf);
  write_buffer1_to_memory(uint32_t page);
  write_buffer2_to_memory(uint32_t page);
  write_buffer1_to_memory_no_erase(uint32_t page);
  write_buffer2_to_memory_no_erase(uint32_t page);
  erase_page(uint16_t page);*/

  /* wait */
 /* bool is_busy();
  wait();*/
private:
  SPI m_SPI;
  std::uint32_t m_Handle{0};
  std::uint8_t m_DeviceID{0};
  std::uint16_t m_PAGE_SIZE{0};
  bool m_IsInitialized{false};
  static constexpr int m_VIRTUAL_PAGE_SIZE{264};
  static constexpr int m_MAX_SUPPORTED_PAGE_SIZE{1056};
  /* Standardized page assignment
  ** Constants to be used for *_virtual_page functions page parameter
  */
  static constexpr int m_CROM_VIRTUAL_PAGE{0};
  static constexpr int m_PLL_VIRTUAL_PAGE{1};
  static constexpr int m_LICENSE_VIRTUAL_PAGE{2};
  static constexpr int m_VCXO_VIRTUAL_PAGE{3};
  static constexpr int m_ADC_CALIBRATION_0_VIRTUAL_PAGE{4}; /* Currently used in x730 boards */
  static constexpr int m_ADC_CALIBRATION_1_VIRTUAL_PAGE{5}; /* Currently used in x761 boards */
  static constexpr int m_OFFSET_CALIBRATION_VIRTUAL_PAGE{6};
  static constexpr int m_PEAK_SENSING_OFFSETS_VIRTUAL_PAGE{7935 * 2};
  static constexpr int m_PEAK_SENSING_CALIB_START_VIRTUAL_PAGE{7936 * 2};
  static constexpr int m_PEAK_SENSING_CALIB_STOP_VIRTUAL_PAGE{8191 * 2 + 1};
  
  /* Read Commands */
  static constexpr int m_MAIN_MEM_PAGE_READ_CMD{0xD2};
  static constexpr int m_CONTINOUS_ARRAY_READ_LEGACY{0xE8};
  static constexpr int m_CONTINOUS_ARRAY_READ_LOW_FREQ{0x03};
  static constexpr int m_CONTINOUS_ARRAY_READ_HIGH_FREQ{0x0B};
  static constexpr int m_BUFFER_1_READ_LOW_FREQ{0xD1};
  static constexpr int m_BUFFER_2_READ_LOW_FREQ{0xD3};
  static constexpr int m_BUFFER_1_READ{0xD4};
  static constexpr int m_BUFFER_2_READ{0xD6};

  /* Program and Erase Command */
  static constexpr int m_BUFFER_1_WRITE{0x84};
  static constexpr int m_BUFFER_2_WRITE{0x87};
  static constexpr int m_BUFFER_1_MAIN_MEM_PAGE_PROG_BE{0x83}; /* w/ Build-in erase */
  static constexpr int m_BUFFER_2_MAIN_MEM_PAGE_PROG_BE{0x86}; /* w/ Build-in erase */
  static constexpr int m_BUFFER_1_MAIN_MEM_PAGE_PROG{0x88};    /* w/o Build-in erase */
  static constexpr int m_BUFFER_2_MAIN_MEM_PAGE_PROG{0x89};    /* w/o Build-in erase */
  static constexpr int m_PAGE_ERASE{0x81};
  static constexpr int m_BLOCK_ERASE{0x50};
  static constexpr int m_SECTOR_ERASE{0x7C};
  static constexpr int m_CHIP_ERASE_0{0xC7};
  static constexpr int m_CHIP_ERASE_1{0x94};
  static constexpr int m_CHIP_ERASE_2{0x80};
  static constexpr int m_CHIP_ERASE_3{0x9A};
  static constexpr int m_MAIN_MEM_PAGE_PROG_TH_BUF_1_CMD{0x82};
  static constexpr int m_MAIN_MEM_PAGE_PROG_TH_BUF_2_CMD{0x85};

  /* Protection and Security Commands */
  static constexpr int m_ENABLE_SECTOR_PROTECTION_0{0x3D};
  static constexpr int m_ENABLE_SECTOR_PROTECTION_1{0x2A};
  static constexpr int m_ENABLE_SECTOR_PROTECTION_2{0x7F};
  static constexpr int m_ENABLE_SECTOR_PROTECTION_3{0xA9};

  static constexpr int m_DISABLE_SECTOR_PROTECTION_0{0x3D};
  static constexpr int m_DISABLE_SECTOR_PROTECTION_1{0x2A};
  static constexpr int m_DISABLE_SECTOR_PROTECTION_2{0x7F};
  static constexpr int m_DISABLE_SECTOR_PROTECTION_3{0xA9};

  static constexpr int m_ERASE_SECTOR_PROTECTION_REG_0{0x3D};
  static constexpr int m_ERASE_SECTOR_PROTECTION_REG_1{0x2A};
  static constexpr int m_ERASE_SECTOR_PROTECTION_REG_2{0x7F};
  static constexpr int m_ERASE_SECTOR_PROTECTION_REG_3{0xCF};

  static constexpr int m_PROGRAM_SECTOR_PROTECTION_REG_0{0x3D};
  static constexpr int m_PROGRAM_SECTOR_PROTECTION_REG_1{0x2A};
  static constexpr int m_PROGRAM_SECTOR_PROTECTION_REG_2{0x7F};
  static constexpr int m_PROGRAM_SECTOR_PROTECTION_REG_3{0xFC};

  static constexpr int m_READ_SECTOR_PROTECTION_REG{0x32};

  static constexpr int m_SECTOR_LOCKDOWN_0{0x3D};
  static constexpr int m_SECTOR_LOCKDOWN_1{0x2A};
  static constexpr int m_SECTOR_LOCKDOWN_2{0x7F};
  static constexpr int m_SECTOR_LOCKDOWN_3{0x30};

  static constexpr int m_READ_SECTOR_LOCKDOWN_REG{0x35};

  static constexpr int m_PROGRAM_SECUTITY_REG_0{0x9B};
  static constexpr int m_PROGRAM_SECUTITY_REG_1{0x00};
  static constexpr int m_PROGRAM_SECUTITY_REG_2{0x00};
  static constexpr int m_PROGRAM_SECUTITY_REG_3{0x00};
  static constexpr int m_READ_SECURITY_REGISTER_CMD{0x77};

  /* Additional commands */
  static constexpr int m_MAIN_MEMORY_PAGE_TO_BUFFER1_XFER{0x53};
  static constexpr int m_MAIN_MEMORY_PAGE_TO_BUFFER2_XFER{0x55};
  static constexpr int m_MAIN_MEMORY_PAGE_TO_BUFFER_1_COMPARE{0x60};
  static constexpr int m_MAIN_MEMORY_PAGE_TO_BUFFER_2_COMPARE{0x61};
  static constexpr int m_AUTO_PAGE_REWRITE_THROUGH_BUFFER_1{0x58};
  static constexpr int m_AUTO_PAGE_REWRITE_THROUGH_BUFFER_2{0x59};
  static constexpr int m_DEEP_PWER_DOWN{0xB9};
  static constexpr int m_RESUME_FROM_DEEP_POWER_DOWN{0xAB};
  static constexpr int m_STATUS_READ_CMD{0xD7};
  static constexpr int m_READ_MANUFACTURER_DEVICE_ID{0x9F};

  /* Legacy commands
  * NOT RECCOMMENDED FOR NEW DESIGNS
  */
  static constexpr int m_LEGACY_BUFFER_1_READ{0x54};
  static constexpr int m_LEGACY_BUFFER_2_READ{0x56};
  static constexpr int m_LEGACY_MAIN_MEMORY_PAGE_READ{0x52};
  static constexpr int m_LEGACY_CONTINOUS_ARRAY_READ{0x68};
  static constexpr int m_LEGACY_STATUS_REGISTER_READ{0x57};
 
  static constexpr int m_SPIFLASH_DEVICEID_64MBIT{0x28};
  static constexpr int m_SPIFLASH_DEVICEID_32MBIT{0x27};
  static constexpr int m_SPIFLASH_DEVICEID_8MBIT{0x25};

  
};

}

#endif
