/*
 * C++ Compatibility Layer for Prism Parser
 *
 * This header provides compatibility macros to allow C99 code to compile
 * with C++ compilers. The main issues addressed are:
 * 1. Designated initializers with enum indices (C99 feature, not in C++17)
 * 2. Compound literals (C99 feature, not in C++)
 *
 * Strategy:
 * - For C compilation: Pass through unchanged (use C99 features)
 * - For C++ compilation: Use alternative syntax
 */

#ifndef PRISM_CPP_COMPAT_H
#define PRISM_CPP_COMPAT_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Compound Literal Compatibility
 *
 * C99: (type) { .field = value }
 * C++: Use temporary variable or constructor syntax
 */
#ifdef __cplusplus
    /* C++ mode: Use brace initialization without cast */
    #define PM_COMPOUND_LITERAL(type, ...) type { __VA_ARGS__ }
#else
    /* C mode: Use compound literal as-is */
    #define PM_COMPOUND_LITERAL(type, ...) (type) { __VA_ARGS__ }
#endif

/*
 * Designated Initializer Compatibility
 *
 * For array initialization with enum indices:
 * C99: [ENUM_VALUE] = { ... }
 * C++: Need to initialize in order or use runtime initialization
 */
#ifdef __cplusplus
    /* C++ mode: Signals that conditional compilation is needed */
    #define PRISM_CPP_MODE 1
#else
    /* C mode: Normal designated initializers work */
    #define PRISM_CPP_MODE 0
#endif

#ifdef __cplusplus
}
#endif

#endif /* PRISM_CPP_COMPAT_H */
