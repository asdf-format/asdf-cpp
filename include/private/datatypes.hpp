#pragma once

#include <string>
#include <cstdint>
#include <climits>
#include <assert.h>


#define DECLARE_DTYPE_TO_STRING(TYPE, STRING)   \
    template <>                                 \
    inline std::string dtype_to_string<TYPE>()  \
    {                                           \
        return std::string(STRING);             \
    }


template <typename T> std::string dtype_to_string(void);

DECLARE_DTYPE_TO_STRING(char,       "int8");
DECLARE_DTYPE_TO_STRING(long,       "int64");

DECLARE_DTYPE_TO_STRING(int8_t,     "int8");
DECLARE_DTYPE_TO_STRING(int16_t,    "int16");
DECLARE_DTYPE_TO_STRING(int32_t,    "int32");
DECLARE_DTYPE_TO_STRING(int64_t,    "int64");

DECLARE_DTYPE_TO_STRING(uint8_t,    "uint8");
DECLARE_DTYPE_TO_STRING(uint16_t,   "uint16");
DECLARE_DTYPE_TO_STRING(uint32_t,   "uint32");
DECLARE_DTYPE_TO_STRING(uint64_t,   "uint64");

/* Sanity checks to ensure cross-platform consistency */
static_assert(CHAR_BIT * sizeof(float) == 32, "Unexpected size for float datatype");
static_assert(CHAR_BIT * sizeof(double) == 64, "Unexpected size for double datatype");
DECLARE_DTYPE_TO_STRING(float,      "float32");
DECLARE_DTYPE_TO_STRING(double,     "float64");
