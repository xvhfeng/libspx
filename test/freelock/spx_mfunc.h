#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

#include "spx_ctype.h"

#if Spx32
#define SpxAlignSize 4
#elif Spx64
#define SpxAlignSize 8
#else
#define SpxAlignSize 8
#endif

#define SpxBit SpxAlignSize

/*
 * p + offset,p is the byte ptr
 */
__SpxPrivate__ __SpxInline__ void *spx_memptr_add(const void *p, const i32_t offset) {
    return (void *) (((char *) p) + offset);
}

__SpxPrivate__ __SpxInline__ void *spx_memptr_sub(const void *p, const i32_t offset) {
    return (void *) (((char *) p) - offset);
}

__SpxPrivate__ __SpxInline__ i32_t spx_memptr_diff(const void *p1, const void *p2) {
    return ((char *) p1) - ((char *) p2);
}

/*
 * align off by base
 * if d = 4,a = 8,then result is 8
 */
__SpxPrivate__ __SpxInline__ i32_t spx_align(const i32_t off, const i32_t base) {
    return (((off) + ((base) -1)) & (~((base) -1)));
}

__SpxPrivate__ __SpxInline__ i32_t spx_align_factor(const i32_t off, const i32_t base) {
    return (spx_align(off, base) / base);
}

__SpxPrivate__ __SpxInline__ i32_t spx_sys_align(const i32_t off) {
    return spx_align(off, SpxBit);
}

__SpxPrivate__ __SpxInline__ i32_t spx_sys_align_factor(const i32_t off) {
    return spx_align_factor(off, SpxBit);
}

/*
 * align bit count to 8
 */
__SpxPrivate__ __SpxInline__ i32_t spx_bit_align(const i32_t bitcount) {
    return spx_align(bitcount, 8);
}

/*
 * align bit count to 8 first,then get the bit use the byte count
 */
__SpxPrivate__ __SpxInline__ i32_t spx_bit_to_byte(const i32_t bitcount) {
    return spx_align_factor(spx_bit_align(bitcount), 8);
}

/*
 * bit align to byte first then align to sys memory unit
 */
__SpxPrivate__ __SpxInline__ i32_t spx_bit_to_byte_align(const i32_t bitcount) {
    return spx_sys_align(spx_bit_to_byte(bitcount));
}

#define SpxMin(a, b)  ((a) < (b) ? (a) : (b))
#define SpxMax(a, b) ((a) > (b) ? (a) : (b))

#define SpxAbs(a) ({         \
    typeof(a) a1 = a;        \
    ((a1) < 0 ? -(a1) : a1); \
})

#define SpxCase(t, name, op) t name = (t)(op)
#define SpxCasep(t, newp, op) t *newp = (t *) (op)
#define SpxCaseTo(t, op) ((t)(op))
#define SpxCasepTo(t, op) ((t *) (op))

#ifndef container_of
#define container_of(ptr, type, member) \
    ((type *) ((char *) (ptr) -offsetof(type, member)))
#endif

#ifndef offsetof
#define offsetof(type, member) ((size_t) & (((type *) 0)->member))
#endif

#define spx_container_of(ptr, type, member) container_of(ptr, type, member)
#define spx_offset_of(type, member) offsetof(type, member)

#define SpxPathMode 0777
#define SpxFileMode 0777
#define SpxPathSize 1023
#define SpxFileNameSize 127
#define SpxSuffixDlmt '.'
#define SpxSuffixDlmtString "."
#define SpxSuffixDlmtLen 1
#define SpxPathDlmtLen 1
#define SpxPathDlmtString "/"
#define SpxLineSize 2047
#define SpxLineEndDlmtString "\n"
#define SpxLineEndDlmt '\n'
#define SpxLineEndDlmtLen 1
#define SpxKeyStringSize 255
#define SpxHostNameSize 255

#define SpxGB (1024 * 1024 * 1024)
#define SpxMB (1024 * 1024)
#define SpxKB (1024)

#define SpxSecondTick 1
#define SpxMinuteTick 60
#define SpxHourTick (60 * 60)
#define SpxDayTick (24 * 60 * 60)

#define SpxDiskUnitPB 0
#define SpxDiskUnitTB 1
#define SpxDiskUnitGB 2
#define SpxDiskUnitMB 3
#define SpxDiskUnitKB 4
#define SpxDiskUnitB 5
#define SpxSecondsOfDay (24 * 60 * 60)
#define SpxBoolTransportSize (sizeof(char))

__SpxPrivate__ char *spx_diskunit_desc[] = {
    "PB",
    "TB",
    "GB",
    "MB",
    "KB",
    "B"
};

#define SpxIpv4Length 15

//#define SpxI32Size   (sizeof("-2147483648") - 1)
//#define SpxI64Size   (sizeof("-9223372036854775808") - 1)

#define LF (u_char) 10
#define CR (u_char) 13
#define CRLF "\x0d\x0a"

#define SpxClose(fd)   \
    do {               \
        if (0 != fd) { \
            close(fd); \
            fd = 0;    \
        }              \
    } while (false)

#define SpxSSet(o, p, v) o->p = (v)

#define spx_errno_clear (errno = 0)

#define spx_zero_p(p) memset((p), 0, sizeof(*(p)))
#define spx_zero(v) memset(&(v), 0, sizeof(v))
#define spx_zero_len(p, len) memset((p), 0, (len))

/*
 * spx_lambda(int,(int x, int y){ return x + y; })(1,1)
 */
#define spx_lambda(return_type, function_body) \
    ({ return_type spx_lambda_fn function_body spx_lambda_fn; })

#define likely(x) __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)

//safely exec x * y % p
/* __SpxPrivate__ __SpxInline__ i64_t spx_mul_safely(const i64_t x, const i64_t y, const i64_t p) {
    i64_t ret = 0;
    for (; y; y >= 1) {
        if (y & 1)
            ret = (ret + x) % p;
        x = (x + x) % p;
    }
    return ret;
} */

#ifdef __cplusplus
}
#endif
