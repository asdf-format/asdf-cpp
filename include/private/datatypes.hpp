#pragma once

#include <string>
#include <cstdint>
#include <climits>
#include <assert.h>


#define DECLARE_DTYPE_FUNCTIONS(TYPE, STRING)           \
    template <>                                         \
    inline std::string dtype_to_string<TYPE>()          \
    {                                                   \
        return std::string(STRING);                     \
    }                                                   \
                                                        \
    template <>                                         \
    inline bool dtype_matches<TYPE>(std::string string) \
    {                                                   \
        return string == STRING;                        \
    }


template <typename T> std::string dtype_to_string(void);
template <typename T> bool dtype_matches(std::string string);

DECLARE_DTYPE_FUNCTIONS(char,       "int8");

DECLARE_DTYPE_FUNCTIONS(int8_t,     "int8");
DECLARE_DTYPE_FUNCTIONS(int16_t,    "int16");
DECLARE_DTYPE_FUNCTIONS(int32_t,    "int32");
DECLARE_DTYPE_FUNCTIONS(int64_t,    "int64");

DECLARE_DTYPE_FUNCTIONS(uint8_t,    "uint8");
DECLARE_DTYPE_FUNCTIONS(uint16_t,   "uint16");
DECLARE_DTYPE_FUNCTIONS(uint32_t,   "uint32");
DECLARE_DTYPE_FUNCTIONS(uint64_t,   "uint64");

/* Sanity checks to ensure cross-platform consistency */
static_assert(CHAR_BIT * sizeof(float) == 32, "Unexpected size for float datatype");
static_assert(CHAR_BIT * sizeof(double) == 64, "Unexpected size for double datatype");
DECLARE_DTYPE_FUNCTIONS(float,      "float32");
DECLARE_DTYPE_FUNCTIONS(double,     "float64");

#ifdef __clang__
static_assert(CHAR_BIT * sizeof(long) == 64, "Unexpected size for long datatype");
DECLARE_DTYPE_FUNCTIONS(long,       "int64");
#endif
