#pragma once

#include <array>
#include <cstdint>
#include <vector>

namespace CAEN
{
class SPI
{
public:
  void         select();
  void         unselect();
  void         setHandle(const std::uint32_t& han);
  void         write(const std::uint8_t& data);
  std::uint8_t read();

  void                      write_block(std::vector<std::uint8_t>& buf);
  std::vector<std::uint8_t> read_block(const std::uint32_t& len);

private:
  static constexpr int                                 m_MAX_SUPPORTED_PAGE_SIZE{1056};
  static constexpr int                                 m_SPI_SELECT_REG_ADDR{0xEF2C};
  static constexpr int                                 m_SPI_DATA_REG_ADDR{0xEF30};
  std::uint32_t                                        m_Handle{0};
  std::array<std::uint32_t, m_MAX_SUPPORTED_PAGE_SIZE> m_Addrs;
  std::array<int, m_MAX_SUPPORTED_PAGE_SIZE>           m_Errors;
  std::array<std::uint32_t, m_MAX_SUPPORTED_PAGE_SIZE> m_Datas;
};

}  // namespace CAEN
