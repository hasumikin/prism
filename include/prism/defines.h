#ifndef PRISM_DEFINES_H
#define PRISM_DEFINES_H

// This file should be included first by any *.h or *.c in prism

#include <ctype.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// PRISM_EXPORTED_FUNCTION
#ifndef PRISM_EXPORTED_FUNCTION
#   ifdef PRISM_EXPORT_SYMBOLS
#       ifdef _WIN32
#          define PRISM_EXPORTED_FUNCTION __declspec(dllexport) extern
#       else
#          define PRISM_EXPORTED_FUNCTION __attribute__((__visibility__("default"))) extern
#       endif
#   else
#       define PRISM_EXPORTED_FUNCTION
#   endif
#endif

// PRISM_ATTRIBUTE_FORMAT
#if defined(__GNUC__)
#   define PRISM_ATTRIBUTE_FORMAT(string_index, argument_index) __attribute__((format(printf, string_index, argument_index)))
#elif defined(__clang__)
#   define PRISM_ATTRIBUTE_FORMAT(string_index, argument_index) __attribute__((__format__(__printf__, string_index, argument_index)))
#else
#   define PRISM_ATTRIBUTE_FORMAT(string_index, argument_index)
#endif

// PRISM_ATTRIBUTE_UNUSED
#if defined(__GNUC__)
#   define PRISM_ATTRIBUTE_UNUSED __attribute__((unused))
#else
#   define PRISM_ATTRIBUTE_UNUSED
#endif

// inline
#if defined(_MSC_VER) && !defined(inline)
#   define inline __inline
#endif

// Windows versions before 2015 use _snprintf
#if !defined(snprintf) && defined(_MSC_VER) && (_MSC_VER < 1900)
#   define snprintf _snprintf
#endif

#endif
