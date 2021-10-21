#pragma once

#if defined(_WIN32)
  #include <windows.h>    //GetModuleFileNameW
#elif defined(__APPLE__)
  #include <mach-o/dyld.h>
  #include <limits.h>
#else
  #include <limits.h>
  #include <unistd.h>     //readlink
#endif

#include <filesystem>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>

static inline std::filesystem::path getFullExePath()
{
  #if  defined(__APPLE__)
    std::uint32_t bufSize{PATH_MAX + 1};
    char result[bufSize];
    _NSGetExecutablePath(result,&bufSize);
    return std::string(result);
  #elif defined(_WIN32)
    TCHAR result[MAX_PATH]{'\0'};
    GetModuleFileName(NULL, result, MAX_PATH);
    return result;
  #else
    const std::size_t bufSize{PATH_MAX + 1};
    char result[bufSize]{'\0'};
    ssize_t count = int(readlink("/proc/self/exe",result,bufSize - 1));
    return std::string(result, (count>0) ? count : 0);
  #endif
}

static inline std::filesystem::path getExePath()
{
  return getFullExePath().parent_path();
}

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

inline std::string NormalizePath(const std::string& path)
{
  std::size_t found = path.find_last_of("/\\");
  if(found == std::string::npos) return path;
  else if(found == path.size()-1) return path.substr(0,found);
  else return path;
}
