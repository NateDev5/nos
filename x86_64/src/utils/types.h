#pragma once

#include <cstddef>
#include <cstdint>

#define IN
#define OUT
#define IN_OUT

#define INTERRUPT

#define STR_MAX_LEN 512

typedef std::size_t     size_t;
//
typedef uint8_t         u8;
typedef uint16_t        u16;
typedef uint32_t        u32;
typedef uint64_t        u64;
//
typedef uint64_t       *PTRMEM;
typedef const uint64_t *PTRMEM_CONST;
typedef void           *PTR;
//
typedef uint32_t       *INTPTR;
//
typedef float           float32;
typedef double          float64;
//
typedef char           *CHAR_PTR;
typedef const char     *CONST_CHAR_PTR;
