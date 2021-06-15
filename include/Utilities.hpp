#pragma once

#include <sstream>
#include <string>
#include <vector>

//-------------------------------------------------------------------------------------------------
/** Convert std::string to a type
 */
template<class T> inline bool stringToType(const std::string& s, T& t)
{
  std::istringstream iss(s);
  return !(iss >> t).fail();
}

//-------------------------------------------------------------------------------------------------
/** Convert type to std::string
 */
template<class T> inline std::string typeToString(const T& t)
{
  std::ostringstream oss;
  if((oss << t).fail()) throw;
  return oss.str();
}

//-------------------------------------------------------------------------------------------------
template<> inline bool stringToType(const std::string& s, std::string& t)
{
  t = s;
  return true;
}

//-------------------------------------------------------------------------------------------------
template<> inline std::string typeToString(const std::string& t)
{
  return t;
}

//-------------------------------------------------------------------------------------------------
/** Tokenize string with delimiter.
 */
template<typename T> inline void tokenize(const std::string& inputString, std::vector<T>& tokens, const std::string& delimiter = " ")
{
  std::string::size_type lastPos = inputString.find_first_not_of(delimiter, 0);
  std::string::size_type pos     = inputString.find_first_of(delimiter, lastPos);

  while((std::string::npos != pos) || (std::string::npos != lastPos))
  {
    T value;
    if(not stringToType(inputString.substr(lastPos, pos - lastPos), value)) { return; }
    tokens.push_back(value);
    lastPos = inputString.find_first_not_of(delimiter, pos);
    pos     = inputString.find_first_of(delimiter, lastPos);
  }
}

//-------------------------------------------------------------------------------------------------
/** Tokenize string with delimiter.
 */
template<> inline void tokenize(const std::string& inputString, std::vector<std::string>& tokens, const std::string& delimiter)
{
  std::string::size_type lastPos = inputString.find_first_not_of(delimiter, 0);
  std::string::size_type pos     = inputString.find_first_of(delimiter, lastPos);

  while((std::string::npos != pos) || (std::string::npos != lastPos))
  {
    tokens.push_back(inputString.substr(lastPos, pos - lastPos));
    lastPos = inputString.find_first_not_of(delimiter, pos);
    pos     = inputString.find_first_of(delimiter, lastPos);
  }
}
