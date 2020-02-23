#include "SPI.hpp"

#include "CAENComm.h"
#include "CAENCommError.hpp"

#include <vector>

namespace CAEN
{
void SPI::select()
{
  CAENCommError(CAENComm_Write32(m_Handle, m_SPI_SELECT_REG_ADDR, 0x2));
}

void SPI::setHandle(const std::uint32_t& han)
{
  m_Handle = han;
}

void SPI::unselect()
{
  CAENCommError(CAENComm_Write32(m_Handle, m_SPI_SELECT_REG_ADDR, 0x3));
}

std::uint8_t SPI::read()
{
  std::uint32_t reg{0};
  CAENCommError(CAENComm_Read32(m_Handle, m_SPI_DATA_REG_ADDR, &reg));
  reg = (reg & 0xFF);
  return reg;
}

void SPI::write(const std::uint8_t& data)
{
  std::uint32_t reg{0};
  reg = data & 0xFF;
  CAENCommError(CAENComm_Write32(m_Handle, m_SPI_DATA_REG_ADDR, reg));
}

void SPI::write_block(std::vector<std::uint8_t>& buf)
{
  for(std::size_t i = 0; i < buf.size(); ++i)
  {
    m_Addrs[i] = m_SPI_DATA_REG_ADDR;
    m_Datas[i] = (uint32_t)buf[i];
  }
  CAENCommError(CAENComm_MultiWrite32(m_Handle, &m_Addrs[0], buf.size(),
                                      &m_Datas[0],
                                      (CAENComm_ErrorCode*)&m_Errors[0]));
}

std::vector<std::uint8_t> SPI::read_block(const std::uint32_t& len)
{
  std::vector<std::uint8_t> ret;
  ret.reserve(len);
  for(std::size_t i = 0; i < len; ++i) { m_Addrs[i] = m_SPI_DATA_REG_ADDR; }
  CAENCommError(CAENComm_MultiRead32(m_Handle, &m_Addrs[0], len, &m_Datas[0],
                                     (CAENComm_ErrorCode*)&m_Errors[0]));
  for(std::size_t i = 0; i < len; ++i)
  {
    CAENCommError((int)m_Errors[i]);
    ret[i] = (uint8_t)m_Datas[i];
  }
  return ret;
}

}  // namespace CAEN
