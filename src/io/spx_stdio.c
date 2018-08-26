#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "spx_allocator.h"
#include "spx_ctype.h"
#include "spx_log.h"
#include "spx_mfunc.h"
#include "spx_ret.h"
#include "spx_stdio.h"
#include "spx_string.h"

__SpxPublic__ i64_t
spx_stdio_filesize(const string_t path,
                   spx_ret_t *ret) { /*{{{*/
    i64_t filesize = -1;
    struct stat statbuff;
    if (stat(path, &statbuff) < 0) {
        *ret = (spx_ret_t) errno;
        return filesize;
    } else {
        filesize = statbuff.st_size;
    }
    *ret = (spx_ret_t) errno;
    return filesize;
} /*}}}*/

__SpxPublic__ bool_t
spx_stdio_isdir(const string_t path,
                spx_ret_t *ret) { /*{{{*/
    struct stat buf;
    if (-1 == stat(path, &buf)) {
        if (ENOENT == errno) {
            return false;
        }
        *ret = 0 == errno ? SpxRet_NoPermissions : errno;
        return false;
    }
    return S_ISDIR(buf.st_mode);
} /*}}}*/

__SpxPublic__ i64_t
spx_stdio_disk_freesize(const string_t path,
                        spx_ret_t *ret) { /*{{{*/
    struct statfs buf;
    SpxZero(buf);
    if (unlikely(0 > statfs(path, &buf))) {
        *ret = (spx_ret_t) errno;
        return -1;
    }
    //not use buf.f_free,because 5%(ofter) for root
    return (i64_t)(buf.f_bavail * buf.f_bsize);
} /*}}}*/

__SpxPublic__ i64_t
spx_stdio_disk_totalsize(const string_t path,
                         spx_ret_t *ret) { /*{{{*/
    struct statfs buf;
    spx_zero(buf);
    if (unlikely(0 > statfs(path, &buf))) {
        *ret = (spx_ret_t) errno;
        return -1;
    }
    return (i64_t)(buf.f_blocks * buf.f_bsize);
} /*}}}*/

__SpxPublic__ string_t
spx_stdio_dirname(spx_allocator_t *allocator,
                  string_t fullpath,
                  spx_ret_t *ret) { /*{{{*/
    if (spx_string_is_null_or_empty(fullpath)) {
        string_t s = spx_string_new(allocator, ".", strlen("."), ret);
        return s;
    }
    ptr_t p = NULL;
    p = (ptr_t) rindex(s, (int) SPX_OS_SEPARATOR);
    if (!p) {
        string_t s = spx_string_new(allocator, ".", strlen("."), ret);
        return s;
    }
    i32_t len = (i32_t) p - s;
    string_t s = spx_string_new(allocator, s, len, ret);
    return s;
} /*}}}*/

__SpxPublic__ string_t
spx_stdio_filename(spx_allocator_t *allocator,
                   string_t fullpath,
                   spx_ret_t *ret) { /*{{{*/
    if (spx_string_is_null_or_empty(fullpath)) {
        *ret = SpxRet_FileOrFolderNotExist;
        return NULL;
    }
    ptr_t p = NULL;
    p = (ptr_t) rindex(s, (int) SPX_OS_SEPARATOR);
    if (!p) {
        string_t s = spx_string_dup(allocator, fullpath, ret);
        return s;
    }
    i32_t offset = (i32_t) p - s + 1;
    i32_t len = spx_string_len(s) - offset;
    string_t s = spx_string_new(allocator, s + offset, len, ret);
    return s;
} /*}}}*/

__SpxPublic__ string_t
spx_stdio_file_suffix(spx_allocator_t *allocator,
                      const string_t fullname,
                      spx_ret_t *ret) { /*{{{*/
    if (spx_string_is_null_or_empty(fullname)) {
        *ret = SpxRet_ArgumentError;
        return NULL;
    }
    string_t newstr = NULL;
    string_t tmp = fullname;
    ptr_t p = (ptr_t) rindex(tmp, (int) '.');
    if (!p) {
        *ret = SpxRet_FilenameError;
        return NULL;
    }
    i32_t off = p - tmp + 1;
    i32_t len = spx_string_len(s) - off;
    newstr = spx_string_new(allocator, fullname + off, len, ret);
    return newstr;
} /*}}}*/

__SpxPublic__ spx_ret_t
spx_stdio_mkdir(spx_allocator_t *allocator,
                const string_t path) { /*{{{*/
    if (!spx_string_is_exist_chr(path, SPX_OS_SEPARATOR)) {
        if (spx_stdio_isdir(path)) {
            return SpxRet_OK;
        }
        if (spx_string_is_exist_chr(path, ((int) '.'))
            && !spx_string_beginwith_chr(path, '.')) {
            return SpxRet_PathFormatError;
        }
    }

    spx_ret_t ret = SpxRet_OK;
    string_t dupstr = spx_string_dup(allocator, path, &ret);
    spx_log_exp_rtn(allocator->log, SpxLogError,
                    !dupstr, ret, ret,
                    "dup path->%s.", path);
rtn:
    spx_string_free(allocator, dupstr);
    return ret;
} /*}}}*/

__SpxPublic__ spx_fd_t *
spx_stdio_wopen(spx_allocator_t *allocator,
                string_t filename,
                bool_t isclear,
                bool_t is_take_filename,
                spx_cleanup_dtor_fn *cleanup_dtor,
                spx_ret_t *ret) { /*{{{*/
    spx_fd_t *fdentry = spx_cleanup_new(allocator, spx_fd_t,
                                        cleanup_dtor ?: spx_stdio_fd_dtor,
                                        ret);
    spx_log_exp_rtn(allocator->log, SpxLogError,
                    !fdentry, NULL, *ret,
                    "new fd_t for file->%s is fail.", filename);

    spx_errno_clear;
    fdentry->fd = spx_wopen(filename, isclear);
    *ret = errno;
    spx_log_exp_goto(allocator->log, SpxLogError,
                     0 > fdentry->fd, error, *ret,
                     "open file->%s is fail.",
                     filename);
    fdentry->filename = filename;
    fdentry->is_take_filename = is_take_filename;
    return fdentry;

error:
    allocator->entry_free(allocator, &fdentry);
    return NULL;
} /*}}}*/

__SpxPublic__ spx_fd_t *
spx_stdio_ropen(spx_allocator_t *allocator,
                string_t filename,
                bool_t isclear,
                bool_t is_take_filename,
                spx_cleanup_dtor_fn *cleanup_dtor,
                spx_ret_t *ret) { /*{{{*/
    spx_fd_t *fdentry = spx_cleanup_new(allocator, spx_fd_t,
                                        cleanup_dtor ?: spx_stdio_fd_dtor,
                                        ret);
    spx_log_exp_rtn(allocator->log, SpxLogError,
                    !fdentry, NULL, *ret,
                    "new fd_t for file->%s is fail.", filename);

    spx_errno_clear;
    fdentry->fd = spx_ropen(filename);
    *ret = errno;
    spx_log_exp_goto(allocator->log, SpxLogError,
                     0 > fdentry->fd, error, *ret,
                     "open file->%s is fail.",
                     filename);
    fdentry->filename = filename;
    fdentry->is_take_filename = is_take_filename;
    return fdentry;

error:
    allocator->entry_free(allocator, &fdentry);
    return NULL;
} /*}}}*/

__SpxPrivate__ spx_ret_t
spx_stdio_fd_dtor(void *alloc,
                  void **ptr) { /*{{{*/
    SpxCasep(spx_fd_t, fdentry, *ptr);
    SpxCasep(spx_allocator_t, allocator, alloc);
    if (fdentry->is_take_filename && fdentry->filename) {
        spx_string_free(allocator, fdentry->filename);
    }
    if (0 < fdentry->fd) {
        SpxClose(fdentry->fd);
    }
    return SpxRet_OK;
} /*}}}*/

__SpxPublic__ spx_mmap_t *
spx_stdio_mmap(spx_allocator_t allocator,
               string_t filename,
               sint_t begin,
               sint_t len,
               sint_t pagesizeB,
               bool_t is_take_filename,
               spx_cleanup_dtor_fn *cleanup_dtor,
               spx_ret_t *ret) { /*{{{*/
    spx_mmap_t *map_entry = spx_cleanup_new(allocator, spx_mmap_t,
                                            cleanup_dtor ?: spx_stdio_mmap_dtor,
                                            ret);
    spx_log_exp_rtn(allocator->log, SpxLogError,
                    !map_entry, NULL, *ret,
                    "new mmap entry for file->%s is fail.", filename);

    spx_errno_clear;
    mmap_entry->fd = spx_wopen(filename, false);
    *ret = errno;
    spx_log_exp_goto(allocator->log, SpxLogError,
                     0 > mmap_entry->fd, error, *ret,
                     "open file->%s is fail.",
                     filename);
    sint_t unit = begin / pagesizeB;
    sint_t mbegin = unit * pagesizeB;
    sint_t off = begin - mbegin;
    sint_t mlen = off + len;

    mmap_entry->begin = mbegin;
    mmap_entry->len = mlen;
    mmap_entry->off = off;

    spx_errno_clear;
    mmap_entry->ptr = spx_mmap(mmap_entry->fd, mbegin, mlen);
    *ret = errno;
    spx_log_exp_goto(allocator->log, SpxLogError,
                     MAP_FAILED == mmap_entry->ptr, error, *ret,
                     "mmap file->%s with fd->%d by begin->%d len->%d"
                     "map-begin->%d map-len->%d map-off->%d unit->%d is fail.",
                     filename, mmap_entry->fd, begin, len,
                     mbegin, mlen, off, unit);
    mmap_entry->filename = filename;
    mmap_entry->is_take_filename = is_take_filename;
    return map_entry;

error:
    allocator->entry_free(allocator, &mmap_entry);
    return NULL;
} /*}}}*/

__SpxPrivate__ spx_ret_t
spx_stdio_mmap_dtor(void *alloc,
                    void **ptr) { /*{{{*/
    SpxCasep(spx_mmap_t, map_entry, *ptr);
    SpxCasep(spx_allocator_t, allocator, alloc);
    if (map_entry->is_take_filename && map_entry->filename) {
        spx_string_free(allocator, map_entry->filename);
    }
    if (0 < map_entry->fd) {
        SpxClose(map_entry->fd);
    }
    spx_munmap(map_entry->ptr, map_entry->len);
    return SpxRet_OK;
} /*}}}*/

__SpxPublic__ spx_ret_t
spx_stdio_nonblocking(int fd) { /* {{{*/
    int flags = 0;
    spx_ret_t ret = SpxRet_OK;
    spx_errno_clear;
    if (-1 == (flags = fcntl(fd, F_GETFL))) {
        ret = errno;
        return ret;
    }

    flags |= O_NONBLOCK;
    if (-1 == fcntl(fd, F_SETFL, flags)) {
        ret = errno;
        return ret;
    }
    return ret;
} /* }}}*/

/*
 *   blocking and must read buffer with size,or report error
 *   len is the really read length
 */
__SpxPublic__ spx_ret_t
spx_read_blocking(const int fd,
                  char *buf,
                  const sint_t totallen,
                  sint_t *len) { /* {{{*/
    spx_ret_t ret = SpxRet_OK;
    sint_t rc = 0;
    sint_t recvbytes = 0;
    while (recvbytes < totallen) {
        spx_errno_clear;
        rc = read(fd, buf + recvbytes, totallen - recvbytes);
        if (0 > rc) {
            if (spx_retry(errno)) {
                continue;
            }
            ret = errno;
            break;
        } else if (0 == rc) {
            break;
        } else {
            recvbytes += rc;
        }
    }
    *len = recvbytes;
    return ret;
} /* }}}*/

/*
 *  * blocking and must write buffer with size,or report error
 *   * len is the really send length
 *    */
__SpxPublic__ spx_ret_t
spx_write_blocking(const int fd,
                   char *buf,
                   const sint_t totallen,
                   sint_t *len) { /* {{{*/
    spx_ret_t ret = SpxRet_OK;
    sint_t rc = 0;
    sint_t writebytes = 0;
    while (writebytes < totallen) {
        spx_errno_clear;
        rc = write(fd, buf + writebytes, totallen - writebytes);
        if (0 > rc) {
            if (spx_retry(errno)) {
                continue;
            }
            ret = errno;
            break;
        } else if (0 == rc) {
            break;
        } else {
            writebytes += rc;
        }
    }
    *len = writebytes;
    return ret;
} /* }}}*/

/*
 *  * read from fd to buf with length,length is maxsize,
 *   * len is the really length
 *    * read data only once
 *    */
__SpxPublic__ spx_ret_t
spx_read_once(const int fd,
              char *buf,
              const sint_t maxlen,
              sint_t *len) { /* {{{*/
    spx_ret_t ret = SpxRet_OK;
    int rc = 0;
    spx_errno_clear;
    rc = read(fd, buf, maxlen);
    if (0 > rc) {
        ret = errno;
    } else {
        *len = rc;
    }
    return ret;
} /* }}}*/

/*
 *  * write buf to fd  with length,length is maxsize
 *   * len is the really length
 *    * write date only once
 *     */
__SpxPublic__ spx_ret_t
spx_write_once(const int fd,
               char *buf,
               const sint_t maxlen,
               sint_t *len) { /* {{{*/
    spx_ret_t ret = SpxRet_OK;
    int rc = 0;
    spx_errno_clear;
    rc = write(fd, ((char *) buf), maxlen);
    if (0 > rc) {
        ret = errno;
    } else {
        *len = rc;
    }
    return ret;
} /* }}}*/

#ifdef SpxLinux
/*
 *   sendfile once
 *   begin: sendfile begin offset
 *   wantsize: size for wanting send
 *   len : really send size
 */

__SpxPublic__ spx_ret_t
spx_sendfile_once(const int sock,
                  const int fd,
                  const sint_t begin,
                  const sint_t maxlen,
                  sint_t *len) { /* {{{*/
    spx_ret_t ret = SpxRet_OK;
    int rc = 0;
    spx_errno_clear;
    rc = sendfile(sock, fd, (off_t *) &(begin), maxlen);
    if (0 > rc) {
        ret = errno;
    } else {
        *len = rc;
    }
    return ret;
} /* }}}*/

/*
 *  sendfile blocking to wantsize
 *  begin: sendfile begin offset
 *  wantsize: size for wanting send
 *  len : really send size
  */
__SpxPublic__ spx_ret_t
spx_sendfile_blocking(const int sock,
                      const int fd,
                      const sint_t begin,
                      const sint_t maxlen,
                      const sint_t *len) { /* {{{*/
    err_t err = 0;
    sint_t sendbytes = 0;
    int rc = 0;
    sint_t offset = begin;
    while (sendbytes < maxlen) {
        spx_errno_clear;
        rc = sendfile(sock, fd, (off_t *) &(offset), maxlen);
        if (0 > rc) {
            if (spx_retry(errno)) {
                continue;
            }
            ret = errno;
            break;
        } else if (0 == rc) {
            break;
        } else {
            sendbytes += rc;
            offset += rc;
        }
    }
    *len = sendbytes;
    return ret;
} /* }}}*/
#endif

__SpxPublic__ spx_ret_t
spx_fwrite_string_blocking(FILE *fp,
                           char *s,
                           sint_t maxlen,
                           sint_t *len) { /* {{{*/
    spx_ret_t ret = SpxRet_OK;
    i64_t rc = 0;
    sint_t writebytes = 0;
    while (writebytes < maxlen) {
        spx_errno_clear;
        rc = fwrite(s + writebytes, sizeof(char), maxlen - writebytes, fp);
        if (0 > rc) {
            if (spx_retry(errno)) {
                continue;
            }
            ret = errno;
            break;
        } else if (0 == rc) {
            break;
        } else {
            writebytes += rc;
        }
    }
    *len = writebytes;
    return ret;
} /* }}}*/

//smartptr
__SpxPublic__ __SpxSmartptrType__(spx_fd_t *)
    spx_stdio_wopen_smartptr(spx_allocator_t *allocator,
                             string_t filename,
                             bool_t isclear,
                             bool_t is_take_filename,
                             spx_smartptr_dtor_fn *smartptr_dtor,
                             spx_ret_t *ret) { /*{{{*/
    spx_smartptr_t spx_fd_t *fdentry = spx_smartptr_new(allocator, spx_fd_t,
                                                        cleanup_dtor ?: spx_stdio_fd_smartptr_dtor,
                                                        ret);
    spx_log_exp_rtn(allocator->log, SpxLogError,
                    !fdentry, NULL, *ret,
                    "new fd_t smartptr for file->%s is fail.", filename);

    spx_errno_clear;
    fdentry->fd = spx_wopen(filename, isclear);
    *ret = errno;
    spx_log_exp_rtn(allocator->log, SpxLogError,
                    0 > fdentry->fd, NULL, *ret,
                    "open file->%s is fail.",
                    filename);
    fdentry->filename = filename;
    fdentry->is_take_filename = is_take_filename;
    return spx_smartptr_rtn(fdentry);

} /*}}}*/

__SpxPublic__ __SpxSmartptrType__(spx_fd_t *)
    spx_stdio_ropen_smartptr(spx_allocator_t *allocator,
                             string_t filename,
                             bool_t isclear,
                             bool_t is_take_filename,
                             spx_smartptr_dtor_fn *smartptr_dtor,
                             spx_ret_t *ret) { /*{{{*/
    spx_smartptr_t spx_fd_t *fdentry = spx_smartptr_new(allocator, spx_fd_t,
                                                        cleanup_dtor ?: spx_stdio_fd_smartptr_dtor,
                                                        ret);
    spx_log_exp_rtn(allocator->log, SpxLogError,
                    !fdentry, NULL, *ret,
                    "new fd_t for file->%s is fail.", filename);

    spx_errno_clear;
    fdentry->fd = spx_ropen(filename);
    *ret = errno;
    spx_log_exp_rtn(allocator->log, SpxLogError,
                    0 > fdentry->fd, NULL, *ret,
                    "open file->%s is fail.",
                    filename);
    fdentry->filename = filename;
    fdentry->is_take_filename = is_take_filename;
    return spx_smartptr_rtn(fdentry);
} /*}}}*/

__SpxPrivate__ void
spx_stdio_fd_smartptr_dtor(void **ptr) { /*{{{*/
    SpxCasep(spx_fd_t, fdentry, *ptr);
    spx_allocator_t *allocator = spx_allocator_from_smarptr(fdentry);
    if (fdentry->is_take_filename && fdentry->filename) {
        spx_string_free(allocator, fdentry->filename);
    }
    if (0 < fdentry->fd) {
        SpxClose(fdentry->fd);
    }
    return;
} /*}}}*/

__SpxPublic__ __SpxSmartptrType__(spx_mmap_t *)
    spx_stdio_mmap_smartptr(spx_allocator_t allocator,
                            string_t filename,
                            sint_t begin,
                            sint_t len,
                            sint_t pagesizeB,
                            bool_t is_take_filename,
                            spx_smartptr_dtor_fn *smartptr_dtor,
                            spx_ret_t *ret) { /*{{{*/
    spx_smartptr_t spx_mmap_t *map_entry = spx_smartptr_new(allocator, spx_mmap_t,
                                                            smartptr_dtor ?: spx_stdio_mmap_smartptr_dtor,
                                                            ret);
    spx_log_exp_rtn(allocator->log, SpxLogError,
                    !map_entry, NULL, *ret,
                    "new mmap entry for file->%s is fail.", filename);

    spx_errno_clear;
    mmap_entry->fd = spx_wopen(filename, false);
    *ret = errno;
    spx_log_exp_rtn(allocator->log, SpxLogError,
                    0 > mmap_entry->fd, NULL, *ret,
                    "open file->%s is fail.",
                    filename);
    sint_t unit = begin / pagesizeB;
    sint_t mbegin = unit * pagesizeB;
    sint_t off = begin - mbegin;
    sint_t mlen = off + len;

    mmap_entry->begin = mbegin;
    mmap_entry->len = mlen;
    mmap_entry->off = off;

    spx_errno_clear;
    mmap_entry->ptr = spx_mmap(mmap_entry->fd, mbegin, mlen);
    *ret = errno;
    spx_log_exp_rtn(allocator->log, SpxLogError,
                    MAP_FAILED == mmap_entry->ptr, NULL, *ret,
                    "mmap file->%s with fd->%d by begin->%d len->%d"
                    "map-begin->%d map-len->%d map-off->%d unit->%d is fail.",
                    filename, mmap_entry->fd, begin, len,
                    mbegin, mlen, off, unit);
    mmap_entry->filename = filename;
    mmap_entry->is_take_filename = is_take_filename;
    return spx_smartptr_rtn(map_entry);
} /*}}}*/

__SpxPrivate__ void
spx_stdio_mmap_smartptr_dtor(void *alloc,
                             void **ptr) { /*{{{*/
    SpxCasep(spx_mmap_t, map_entry, *ptr);
    spx_allocator_t allocator = spx_allocator_from_smartptr(map_entry);
    if (map_entry->is_take_filename && map_entry->filename) {
        spx_string_free(allocator, map_entry->filename);
    }
    if (0 < map_entry->fd) {
        SpxClose(map_entry->fd);
    }
    spx_munmap(map_entry->ptr, map_entry->len);
    return;
} /*}}}*/

_
