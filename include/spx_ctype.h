#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <wchar.h>

#ifndef bool_t
typedef enum {
    false = 0,
    true = 1
} bool_t;
#endif

#ifndef byte_t
typedef signed char byte_t;
#endif

#ifndef ubyte_t
typedef unsigned char ubyte_t;
#endif

#ifndef uchar_t
typedef unsigned char uchar_t;
#endif

#ifndef string_t
typedef char* string_t;
#endif

#ifndef wstring_t
typedef wchar_t* wstring_t;
#endif

#ifndef u64_t
typedef u_int64_t u64_t;
#endif

#ifndef u32_t
typedef u_int32_t u32_t;
#endif

#ifndef u16_t
typedef u_int16_t u16_t;
#endif

#ifndef u8_t
typedef u_int8_t u8_t;
#endif

#ifndef i64_t
typedef int64_t i64_t;
#endif

#ifndef i32_t
typedef int32_t i32_t;
#endif

#ifndef i16_t
typedef int16_t i16_t;
#endif

#ifndef i8_t
typedef int8_t i8_t;
#endif

/*
 * system int type
 * means aligned unit
 */
#ifndef sint_t
#if Spx32
typedef i32_t sint_t;
#define SINT_MAX 0xFFFFFFFF
#elif Spx64
typedef i64_t sint_t;
#define SINT_MAX 0xFFFFFFFFFFFFFFFF
#else
typedef i64_t sint_t;
#define SINT_MAX 0xFFFFFFFFFFFFFFFF
#endif
#endif

#ifndef ptr_t
typedef char* ptr_t;
#endif

#ifndef spx_atomic_t
#define spx_atomic_t volatile
#endif

#ifndef __SpxPrivate__
#define __SpxPrivate__ static
#endif

#ifndef __SpxPublic__
#define __SpxPublic__
#endif

#ifndef __SpxInternal__
#define __SpxInternal__
#endif

#ifdef SpxDEBUG
#define __SpxInline__
#else
#define __SpxInline__ __attribute__((always_inline)) inline
#endif

#ifndef null
#define null NULL
#endif

#ifdef __cplusplus
}
#endif
