# Prism C++ Compilation Compatibility Patch

This document describes the changes needed to compile the Prism parser library (originally C99) with C++ compilers like em++/clang++/g++.

## Overview

The Prism parser library is written in C99 and uses features that are not compatible with C++:
- Designated initializers with enum indices
- Compound literals
- `operator` as a parameter name (C++ keyword)
- Enum type bitwise operations without explicit casting

## Strategy

The patch minimizes changes to the original C99 code by:
1. Using compiler warning suppression for most C99 features
2. Providing C++ compatibility macros for compound literals
3. Renaming C++ reserved keywords where necessary

## Changes Made

### 1. C++ Compatibility Header

**File:** `include/prism/cpp_compat.h` (NEW)

Provides the `PM_COMPOUND_LITERAL` macro that works in both C and C++ modes:
- C mode: Uses compound literals `(type) { ... }`
- C++ mode: Uses brace initialization `type { ... }`

### 2. Include Compatibility Header

**File:** `include/prism/defines.h`

Added at the end (before `#endif`):
```c
#include "prism/cpp_compat.h"
```

### 3. Fix Compound Literals

**File:** `src/diagnostic.c`

Changed 2 compound literal usages to use the `PM_COMPOUND_LITERAL` macro:
- Line ~1071: `pm_diagnostic_list_append()`
- Line ~1116: `pm_diagnostic_list_append_format()`

Changed from:
```c
*diagnostic = (pm_diagnostic_t) {
    .location = { start, end },
    ...
};
```

To:
```c
*diagnostic = PM_COMPOUND_LITERAL(pm_diagnostic_t,
    .location = { start, end },
    ...
);
```

### 4. Compiler Flags

When compiling with C++ compiler, add these flags to suppress C99-specific warnings:

```makefile
CXXFLAGS := -Wno-c99-designator \
            -Wno-c99-extensions \
            -Wno-c++20-designator \
            -Wno-reorder-init-list \
            -Wno-missing-field-initializers \
            -Wno-varargs \
            -Wno-deprecated
```

Example usage:
```bash
CC=em++ AR=emar CFLAGS="-Wno-c99-designator -Wno-c99-extensions -Wno-c++20-designator -Wno-reorder-init-list -Wno-missing-field-initializers -Wno-varargs -Wno-deprecated" make build/libprism.a
```

## Completed Work

### ✅ C++ Compatibility Header
Created `include/prism/cpp_compat.h` with `PM_COMPOUND_LITERAL` macro.

### ✅ diagnostic.c - Fully Fixed
- Modified 2 compound literal usages
- Successfully compiles with C++ compiler

### ✅ prism.c - Major Fixes Applied
- Replaced `operator` keyword in ~90% of occurrences:
  - Function parameters: `*operator)` → `*operator_tok)`
  - Pointer access: `operator->` → `operator_tok->`
  - Local variables: `pm_token_t operator;` → `pm_token_t operator_tok;`
- Fixed enum bitwise operations (5 locations)
- Modified PM_STRING_EMPTY macro in `include/prism/util/pm_string.h`

### ✅ Compiler Flags Identified
```bash
-Wno-c99-designator
-Wno-c99-extensions
-Wno-c++20-designator
-Wno-reorder-init-list
-Wno-missing-field-initializers
-Wno-varargs
-Wno-deprecated
-Wno-c++11-narrowing
-Wno-address-of-temporary
```

## Final Status (Updated)

### ✅ Fully Completed Modifications

1. **`include/prism/cpp_compat.h`** - C++ compatibility macro layer
2. **`include/prism/defines.h`** - Include cpp_compat.h
3. **`include/prism/util/pm_string.h`** - Moved enum outside struct as `typedef enum pm_string_type_enum`
4. **`include/prism/parser.h`** - Moved enum outside struct as `typedef enum pm_lex_mode_enum`
5. **`src/diagnostic.c`** - All compound literals converted (100%)
6. **`src/prism.c`** - Major fixes:
   - `operator` keyword → `operator_tok` (100% complete)
   - xmalloc/memchr type casts (100% complete)
   - enum bitwise operations with casts (100% complete)
   - Shift operations: `1U` → `1ULL` for 64-bit enum values (100% complete)
   - Binding power arithmetic with explicit casts (100% complete)
   - Switch statement fixes for `case '$':` and `case '[':` (partial)
7. **`Makefile`** - Auto-detect C++ compiler and disable `-Werror` for C++

### 🔧 Remaining Issue: Switch Statement Variable Initialization

**Problem:** C++ does not allow jumping to a case label when a previous case initializes variables without enclosing them in a block scope.

**Example:**
```c
switch (token) {
    case '[':
        pm_token_type_t type = PM_TOKEN_BRACKET_LEFT;  // Variable initialization
        // ... code ...
        LEX(type);
    case ']':  // ERROR in C++: jump bypasses variable initialization
        // ... code ...
}

// Must be:
switch (token) {
    case '[': {  // Add braces
        pm_token_type_t type = PM_TOKEN_BRACKET_LEFT;
        // ... code ...
        LEX(type);
    }
    case ']':  // OK now
        // ... code ...
}
```

**Affected:** ~15+ locations in `src/prism.c` in large switch statements

**Impact:** This is a C++ language requirement and cannot be disabled with compiler flags (not even `-fpermissive`).

**Status:** Partially fixed (2 cases wrapped with braces: `case '$'` and `case '['` at lines 9402 and 11025)

**Solutions:**
1. **Wrap each affected case with braces** `{}` to create block scope (recommended)
2. **Move variable declarations** to the top of the switch statement
3. **Use a script** to automatically detect and fix all occurrences

## Summary of Approach

This patch takes a pragmatic approach to C++ compilation:

1. **Minimal header changes**: Only converted anonymous enums to typedef enums
2. **Macro-based compatibility**: Used `PM_COMPOUND_LITERAL` for C99 compound literals
3. **Systematic replacements**: Used sed for bulk replacements (operator, casts, shifts)
4. **Compiler flag adjustments**: Modified Makefile to handle C++ compilation gracefully
5. **Documentation**: Comprehensive tracking of all changes and remaining issues

## Compilation Progress

### What Works
- ✅ All header files compile successfully
- ✅ diagnostic.c compiles successfully
- ✅ encoding.c, node.c, options.c, pack.c, prettyprint.c compile successfully
- ⚠️ prism.c compiles with ~15 remaining switch statement errors

### Build Command
```bash
CC=em++ AR=emar CFLAGS="-Wno-deprecated -Wno-c99-designator -Wno-varargs -Wno-c99-extensions -Wno-c++20-designator -Wno-reorder-init-list -Wno-missing-field-initializers -Wno-c++11-narrowing -Wno-address-of-temporary" make build/libprism.a
```

## Files Modified Summary

| File | Status | Description |
|------|--------|-------------|
| `include/prism/cpp_compat.h` | ✅ COMPLETE | C++ compatibility macros |
| `include/prism/defines.h` | ✅ COMPLETE | Include cpp_compat.h |
| `include/prism/util/pm_string.h` | ✅ COMPLETE | Moved enum to `typedef enum pm_string_type_enum` |
| `include/prism/parser.h` | ✅ COMPLETE | Moved enum to `typedef enum pm_lex_mode_enum` |
| `src/diagnostic.c` | ✅ COMPLETE | Use PM_COMPOUND_LITERAL macro (2 places) |
| `src/prism.c` | ✅ COMPLETE | Renamed all `operator` → `operator_tok` |
| `src/prism.c` | ✅ COMPLETE | Fixed enum bitwise operations with casts |
| `src/prism.c` | ✅ COMPLETE | Added xmalloc/memchr/pm_memchr casts |
| `src/prism.c` | ✅ COMPLETE | Changed `1U` → `1ULL` for 64-bit shift operations |
| `src/prism.c` | ✅ COMPLETE | Added casts for binding power arithmetic |
| `src/prism.c` | ⚠️ PARTIAL | Removed invalid `equal_loc` field |
| `src/prism.c` | ⚠️ PARTIAL | Fixed 2 switch cases (`case '$'`, `case '['`) - ~15+ more needed |
| `Makefile` | ✅ COMPLETE | Auto-detect C++ and disable `-Werror` |

## Testing

### Test C Compilation (should still work)
```bash
CC=cc make clean build/libprism.a
```

### Test C++ Compilation
```bash
CC=em++ AR=emar CFLAGS="-Wno-c99-designator -Wno-c99-extensions -Wno-c++20-designator -Wno-reorder-init-list -Wno-missing-field-initializers -Wno-varargs -Wno-deprecated" make clean build/libprism.a
```

## Maintenance Notes

### For Future Updates

When merging upstream changes from the Prism project:

1. **cpp_compat.h**: This file should remain stable
2. **defines.h**: Ensure the `#include "prism/cpp_compat.h"` line is preserved
3. **diagnostic.c**: If regenerated from templates, reapply PM_COMPOUND_LITERAL changes
4. **prism.c**: If regenerated, reapply `operator` → `operator_tok` renaming

### Template Files

Note that some files are generated from ERB templates:
- `src/diagnostic.c` ← `templates/src/diagnostic.c.erb`
- `src/node.c` ← `templates/src/node.c.erb`

For permanent fixes, modify the template files as well.

## Rationale

### Why Not Fully Rewrite for C++?

The goal is to maintain compatibility with the upstream Prism project (which is C99) while allowing C++ compilation for embedded systems (R2P2 project using Emscripten). Minimal changes make it easier to:
- Merge upstream updates
- Potentially contribute patches back to Prism
- Switch between C and C++ compilation as needed

### Why Use Warning Suppression?

GNU C and Clang support C99 designated initializers as an extension in C++ mode. While not standard C++, these extensions are widely supported and allow us to keep the original array initialization code intact. This is much more maintainable than converting hundreds of array entries to C++-compatible syntax.

## References

- Prism Parser Project: https://github.com/ruby/prism
- C99 Features: ISO/IEC 9899:1999
- C++ Standards: ISO/IEC 14882
- GNU C Extensions: https://gcc.gnu.org/onlinedocs/gcc/C-Extensions.html
