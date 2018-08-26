#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

#include "spx_allocator.h"
#include "spx_ctype.h"
#include "spx_log.h"

#define SPX_OS_SEPARATOR '/'

typedef struct {
    spx_klist_node_t chain;
    string_t filename;
    int fd;
    bool_t is_take_filename;
} spx_fd_t;

typedef struct {
    spx_klist_node_t chain;
    string_t filename;
    bool_t is_take_filename;
    FILE *file;
} spx_file_t;

typedef struct {
    spx_klist_node_t chain;
    int fd;
    i32_t off;
    sint_t begin;
    sint_t len;
    char *ptr;
    string_t filename;
    bool_t is_take_filename;
} spx_mmap_t;

#define spx_wopen(filename, is_clear)                \
    open(filename,                                   \
         is_clear                                    \
             ? O_RDWR | O_CREAT | O_TRUNC | O_APPEND \
             : O_RDWR | O_CREAT | O_APPEND,          \
         0744)

#define spx_ropen(filename) open(filename, O_RDONLY)

#define spx_wfopen(filename, is_clear) \
    fopen(filename, is_clear ? "w" : "a")

#define spx_rfopen(filename) fopen(filename, "r")

#define spx_mmap(fd, begin, len) \
    mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, begin)

#define spx_munmap(mptr, len)    \
    if (NULL != mptr && len > 0) \
        do {                     \
            munmap(mptr, len);   \
            mptr = NULL;         \
    } while (false)

#define spx_exist(filename) (0 == access(filename, F_OK))

#define spx_readable(filename) (0 == access(filename, R_OK))

#define spx_writable(filename) (0 == access(filename, W_OK))

#define spx_executable(filename) (0 == access(filename, X_OK))

#ifdef __cplusplus
}
#endif
