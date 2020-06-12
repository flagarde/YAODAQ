#pragma once

#include "Internal.hpp"
#include "SourceLocation.hpp"

#include <exception>
#include <string>

enum class StatusCode : int_least32_t;

class Exception: public std::exception
{
public:
  Exception(const StatusCode& statusCode, const std::string& description, const SourceLocation& location = SourceLocation());
  Exception(const Exception& e) = default;
  virtual ~Exception() noexcept override;
  virtual const char*  what() const noexcept override;
  const uint_least32_t getLine() const;
  const uint_least32_t getColumn() const;
  const char*          getFileName() const;
  const char*          getFunctionName() const;
  const char*          getDescription() const;
  const int_least32_t  getCode() const;

protected:
  Exception(const int_least32_t& code, const std::string& description, const SourceLocation& location = SourceLocation());

private:
  std::string         m_Description{""};
  std::string         m_Message{""};
  const int_least32_t m_Code{0};
  Exception() = delete;
  void           constructMessage() noexcept;
  SourceLocation m_SourceLocation;
};
