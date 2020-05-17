#if defined(_MSC_VER)
  #define DISABLE_WARNING_PMFCONVERSIONS

#elif defined(__clang__) || defined(__llvm__)
  #define DISABLE_WARNING_PMFCONVERSIONS

#elif defined(__GNUC__)
  #define DISABLE_WARNING_PMFCONVERSIONS DISABLE_WARNING(-Wpmf-conversions)

#else
  #define DISABLE_WARNING_UNREFERENCED_FUNCTION
#endif
