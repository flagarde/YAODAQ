#pragma once

#include "Internal.hpp"
#include "SourceLocation.hpp"

#include <exception>
#include <string>
#include <tuple>

#include "fmt/color.h"

enum class StatusCode : int_least32_t;

class Exception: public std::exception
{
public:
  Exception() = delete;
  //Ugly but should work
  //with std::make_tuple
  template<typename... Ta>
  Exception(const StatusCode& statusCode, const std::string& format, const std::tuple<Ta...>& t, const SourceLocation& location = SourceLocation())
      : m_Code(static_cast<int_least32_t>(statusCode)), m_Description(fmt::format(format, t)), m_SourceLocation(location)
  {
    constructMessage();
  }
  //1
  template<typename T0>
  Exception(const StatusCode& statusCode, const std::string& format, T0&& t0, const SourceLocation& location = SourceLocation())
      : m_Code(static_cast<int_least32_t>(statusCode)), m_Description(fmt::format(format, t0)), m_SourceLocation(location)
  {
    constructMessage();
  }
  //2
  template<typename T0, typename T1>
  Exception(const StatusCode& statusCode, const std::string& format, T0&& t0, T1&& t1, const SourceLocation& location = SourceLocation())
      : m_Code(static_cast<int_least32_t>(statusCode)), m_Description(fmt::format(format, t0, t1)), m_SourceLocation(location)
  {
    constructMessage();
  }
  //3
  template<typename T0, typename T1, typename T2>
  Exception(const StatusCode& statusCode, const std::string& format, T0&& t0, T1&& t1, T2&& t2, const SourceLocation& location = SourceLocation())
      : m_Code(static_cast<int_least32_t>(statusCode)), m_Description(fmt::format(format, t0, t1, t2)), m_SourceLocation(location)
  {
    constructMessage();
  }
  //4
  template<typename T0, typename T1, typename T2, typename T3>
  Exception(const StatusCode& statusCode, const std::string& format, T0&& t0, T1&& t1, T2&& t2, T3&& t3, const SourceLocation& location = SourceLocation())
      : m_Code(static_cast<int_least32_t>(statusCode)), m_Description(fmt::format(format, t0, t1, t2, t3)), m_SourceLocation(location)
  {
    constructMessage();
  }
  //5
  template<typename T0, typename T1, typename T2, typename T3, typename T4>
  Exception(const StatusCode& statusCode, const std::string& format, T0&& t0, T1&& t1, T2&& t2, T3&& t3, T4&& t4, const SourceLocation& location = SourceLocation())
      : m_Code(static_cast<int_least32_t>(statusCode)), m_Description(fmt::format(format, t0, t1, t2, t3, t4)), m_SourceLocation(location)
  {
    constructMessage();
  }
  //6
  template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
  Exception(const StatusCode& statusCode, const std::string& format, T0&& t0, T1&& t1, T2&& t2, T3&& t3, T4&& t4, T5&& t5, const SourceLocation& location = SourceLocation())
      : m_Code(static_cast<int_least32_t>(statusCode)), m_Description(fmt::format(format, t0, t1, t2, t3, t4, t5)), m_SourceLocation(location)
  {
    constructMessage();
  }
  //7
  template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
  Exception(const StatusCode& statusCode, const std::string& format, T0&& t0, T1&& t1, T2&& t2, T3&& t3, T4&& t4, T5&& t5, T6&& t6, const SourceLocation& location = SourceLocation())
      : m_Code(static_cast<int_least32_t>(statusCode)), m_Description(fmt::format(format, t0, t1, t2, t3, t4, t5, t6)), m_SourceLocation(location)
  {
    constructMessage();
  }
  //8
  template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
  Exception(const StatusCode& statusCode, const std::string& format, T0&& t0, T1&& t1, T2&& t2, T3&& t3, T4&& t4, T5&& t5, T6&& t6, T7&& t7, const SourceLocation& location = SourceLocation())
      : m_Code(static_cast<int_least32_t>(statusCode)), m_Description(fmt::format(format, t0, t1, t2, t3, t4, t5, t6, t7)), m_SourceLocation(location)
  {
    constructMessage();
  }
  //9
  template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
  Exception(const StatusCode& statusCode, const std::string& format, T0&& t0, T1&& t1, T2&& t2, T3&& t3, T4&& t4, T5&& t5, T6&& t6, T7&& t7, T8&& t8, const SourceLocation& location = SourceLocation())
      : m_Code(static_cast<int_least32_t>(statusCode)), m_Description(fmt::format(format, t0, t1, t2, t3, t4, t5, t6, t7, t8)), m_SourceLocation(location)
  {
    constructMessage();
  }

  static void setFormat(const std::string& format) { m_Format = format; }

  static void setStyle(const fmt::text_style& style = {}) { m_Style = style; }

  Exception(const StatusCode& statusCode, std::string description, const SourceLocation& loc = SourceLocation());
  ~Exception() noexcept override = default;
  [[nodiscard]] const char*    what() const noexcept override;
  [[nodiscard]] uint_least32_t getLine() const;
  [[nodiscard]] uint_least32_t getColumn() const;
  [[nodiscard]] const char*    getFileName() const;
  [[nodiscard]] const char*    getFunctionName() const;
  [[nodiscard]] const char*    getDescription() const;
  [[nodiscard]] int_least32_t  getCode() const;

protected:
  Exception(const int_least32_t& code, std::string description, const SourceLocation& location = SourceLocation());

private:
  static fmt::text_style m_Style;
  static std::string     m_Format;
  std::string            m_Description;
  std::string            m_Message;
  const int_least32_t    m_Code{0};
  void                   constructMessage();
  SourceLocation         m_SourceLocation;
};
