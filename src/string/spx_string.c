#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "_spx_log.h"
#include "spx_allocator.h"
#include "spx_ctype.h"
#include "spx_debugger.h"
#include "spx_klist.h"
#include "spx_mfunc.h"
#include "spx_ret.h"
#include "spx_string.h"

#define CASE_SPACE \
    ' ' : case '\t' : case '\n' : case '\f' : case '\v' : case '\r'

__SpxPrivate__ __SpxInline__ string_t
spx_string_makeroom(spx_allocator_t *allocator,
                    string_t s,
                    const sint_t explen,
                    spx_ret_t *ret);

__SpxPrivate__ i32_t
spx_string_match_suffix(const char *ptt,
                        const i32_t len,
                        const char bad);

__SpxPrivate__ i32_t
spx_string_rmatch_suffix(const char *ptt,
                         const i32_t len,
                         const char bad);

__SpxPrivate__ __SpxInline__ string_t
spx_string_makeroom(spx_allocator_t *allocator,
                    string_t s,
                    const sint_t explen,
                    spx_ret_t *ret) { /*{{{*/
    spx_entry_t *entry = spx_container_of(s, spx_entry_t, buf);
    string_t newstr = NULL;
    if (SpxEntrySmartptr == entry->flag) {
        spx_entry_smartptr_t *sptr = spx_container_of(s, spx_entry_smartptr_t, buf);
        spx_smartptr_t string_t sp = s;  //auto free string
        sint_t len = sptr->len;
        sint_t busylen = sptr->busylen;
        sint_t newlen = len + explen;
        newstr = spx_string_smartptr(allocator, sp, newlen, ret);
        spx_log_exp_rtn(allocator->log, SpxLogError, newstr, NULL, *ret,
                        "makeroom string to->%d in allocator->%s.",
                        newlen, allocator->name);
        spx_string_update_busylen(newstr, busylen);
    } else {
        sint_t len = sptr->len;
        sint_t busylen = sptr->busylen;
        sint_t newlen = len + explen;
        newstr = spx_string_new(allocator, s, newlen, ret);
        spx_log_exp_rtn(allocator->log, SpxLogError, newstr, NULL, *ret,
                        "makeroom string to->%d in allocator->%s.",
                        newlen, allocator->name);
        spx_string_update_busylen(newstr, busylen);
        spx_string_free(allocator, &s);
    }
    return newstr;
} /*}}}*/

__SpxPublic__ string_t
spx_string_new(spx_allocator_t *allocator,
               const void *init,
               const sint_t len,
               spx_ret_t *ret) { /*{{{*/
    if (NULL == init) {
        return spx_string_newlen(allocator, len, ret);
    }
    sint_t busylen = spx_string_ctxlen(len);
    sint_t size = spx_sys_align(busylen);
    sint_t totalsize = sizeof(spx_entry_t) + size;
    spx_entry_t *entry = spx_entry_newlen(allocator, spx_entry_t,
                                          size, ret);
    spx_log_exp_rtn(allocator->log, SpxLogError, !entry, NULL, *ret,
                    "new string in allocator->%s.", allocator->name);
    memcpy(entry->buf, (char *) init, len);
    spx_entry_base_set(entry, totalsize, busylen, SpxEntryNormal);
    return (string_t) entry->buf;
} /*}}}*/

__SpxPublic__ string_t
spx_string_newlen(spx_allocator_t *allocator,
                  const sint_t len,
                  spx_ret_t *ret) { /*{{{*/
    sint_t busylen = spx_string_ctxlen(len);
    sint_t size = spx_sys_align(busylen);
    sint_t totalsize = sizeof(spx_entry_t) + size;
    spx_entry_t *entry = spx_entry_newlen(allocator, spx_entry_t, size, ret);
    spx_log_exp_rtn(allocator->log, SpxLogError, !entry, NULL, *ret,
                    "new string in allocator->%s.", allocator->name);
    spx_entry_base_set(entry, totalsize, busylen, SpxEntryNormal);
    return (string_t) entry->buf;
} /*}}}*/

__SpxPrivate__ void
spx_string_smartptr_dtor(void **p) { /*{{{*/
    string_t s = (string_t) *p;
    spx_allocator_smartptr_unref(s);
    return;
} /*}}}*/

__SpxPublic__ __SpxSmartptrType__(string_t)
    spx_string_smartptr(spx_allocator_t *allocator,
                        const void *init,
                        const sint_t len,
                        spx_ret_t *ret) { /*{{{*/
    sint_t busylen = spx_string_ctxlen(len);
    sint_t size = spx_sys_align(busylen);
    string_t s = spx_smartptr_newlen_p(allocator, string_t, size,
                                       spx_string_smartptr_dtor, ret);
    spx_log_exp_rtn(allocator->log, SpxLogError, !s, NULL, *ret,
                    "new string in allocator->%s.", allocator->name);
    if (NULL != init) {
        memcpy(s->buf, (char *) init, len);
    }
    spx_string_updatelen(s, busylen);
    return s;
} /*}}}*/

__SpxPublic__ int
spx_string_cmp(bool_t is_case,
               const string_t s,
               const string_t ptt) { /*{{{*/
    return spx_str_cmp(is_case, s, ppt, spx_string_len(ppt));
} /*}}}*/

__SpxPublic__ int
spx_str_cmp(bool_t is_case,
            const string_t s,
            const char *ptt,
            const sint_t len) { /*{{{*/
    sint_t l1, minlen;
    int cmp = 0;

    l1 = spx_string_len(s);
    minlen = (l1 < len) ? l1 : len;

    if (!is_case) {
        cmp = memcmp(s, ptt, minlen);
        return cmp ?: (l1 - l2);
        // return 0 == cmp ? (l1 - l2) : cmp;
    }

    char a, b;
    sint_t i = 0;
    for (; i < minlen; i++) {
        a = *(s + i);
        b = *(ptt + i);
        a = (65 <= a && 90 >= a) ? a + 32 : a;
        b = (65 <= b && 90 >= b) ? b + 32 : b;
        cmp = a - b;
        if (0 != cmp)
            return cmp;
    }
    return cmp ?: (l1 - l2);
} /*}}}*/

__SpxPublic__ string_t
spx_string_cat(spx_allocator_t *allocator,
               string_t s1,
               const string_t s2,
               spx_ret_t *ret) { /*{{{*/
    return spx_str_cat(allocator, s1, s2, spx_string_len(s2), ret);
} /*}}}*/

__SpxPublic__ string_t
spx_str_cat(spx_allocator_t *allocator,
            string_t s1,
            const char *s2,
            const sint_t len,
            spx_ret_t *ret) { /*{{{*/
    sint_t freelen = spx_string_freelen(s1);
    sint_t busylen = spx_string_len(s1);
    if (len > freelen) {
        sint_t elen = len - freelen;
        string_t tmp = spx_string_makeroom(allocator, s1, elen, ret);
        spx_log_exp_rtn(allocator->log, SpxLogError,
                        !tmp, NULL, *ret,
                        "make string free room.allocator->%s.",
                        allocator->name);
        s1 = tmp;
    }

    memcpy(s1 + busylen, s2, len);
    spx_string_update_busylen(s1, busylen + len);
    return s1;
} /*}}}*/

/* padding cat s2 to s1 with plen
 * if s2 length less than plen,padding padding
 * if s2 length more than plen,cat s2 with plen
 */
__SpxPublic__ string_t
spx_string_pcat(spx_allocator_t *allocator,
                string_t s1,
                const string_t s2,
                const sint_t plen,
                const char pad) { /*{{{*/
    return spx_str_pcat(allocator, s1, s2,
                        spx_string_len(s2),
                        plen, pad);
} /*}}}*/

__SpxPublic__ string_t
spx_str_pcat(spx_allocator_t *allocator,
             string_t s1,
             const char *s2,
             const sint_t len,
             const sint_t plen,
             const char pad) { /*{{{*/
    sint_t freelen = spx_string_freelen(s1);
    sint_t busylen = spx_string_len(s1);
    if (plen > freelen) {
        sint_t elen = plen - freelen;
        string_t tmp = spx_string_makeroom(allocator, s1, elen, ret);
        spx_log_exp_rtn(allocator->log, SpxLogError,
                        !tmp, NULL, *ret,
                        "make string free room.allocator->%s.",
                        allocator->name);
        s1 = tmp;
    }

    memcpy(s1 + busylen, s2, SpxMin(plen, len));
    if (plen > len) {
        memset(s2 + (busylen + len2), pad, plen - len);
    }
    spx_string_update_busylen(s1, busylen + plen);
    return s1;
} /*}}}*/

__SpxPublic__ string_t
spx_string_nvcat(spx_ret_t *ret,
                 spx_allocator_t *allocator,
                 string_t s,
                 const char *fmt,
                 va_list ap) { /*{{{*/
    va_list cpy;
    char *buf = NULL;
    string_t newstr = NULL;
    sint_t buflen = 16;

    while (1) {
        buf = spx_entry_newlen(allocator->dalloc, buflen, ret);
        spx_log_exp_rtn(allocator->log, SpxLogError,
                        !buf, NULL, *ret,
                        "new string for vncat.");
        buf[ buflen - 2 ] = '\0';
        va_copy(cpy, ap);
        vsnprintf(buf, buflen, fmt, cpy);
        if (buf[ buflen - 2 ] != '\0') {
            allocator->dalloc->entry_free(NULL, buf);
            buflen *= 2;
            continue;
        }
        break;
    }
    newstr = spx_str_cat(allocator, s, buf, strlen(buf), ret);
    allocator->dalloc->entry_free(allocator, buf);
    spx_log_exp_rtn(allocator->log, SpxLogError,
                    !newstr, NULL, *ret,
                    "cat fmt to string in vncat.");
    return newstr;
} /*}}}*/

__SpxPublic__ string_t
spx_string_vcat(spx_ret_t *ret,
                spx_allocator_t *allocator,
                string_t s,
                const char *fmt,
                ...) { /*{{{*/
    va_list ap;
    string_t newstr = NULL;
    va_start(ap, fmt);
    newstr = spx_string_vncat(ret, s, fmt, ap);
    va_end(ap);
    return newstr;
} /*}}}*/

__SpxPublic__ string_t
spx_string_cpy(spx_allocator_t *allocator,
               string_t s1,
               sint_t off1,
               const string_t s2,
               const sint_t off2,
               const sint_t len,
               spx_ret_t *ret) { /*{{{*/
    return spx_str_cpy(allocator, s1,
                       off1, s2, off2,
                       spx_string_len(s2), ret);
} /*}}}*/

__SpxPublic__ string_t
spx_str_cpy(spx_allocator_t *allocator,
            string_t s1,
            const sint_t off1,
            const char *s2,
            const sint_t off2,
            const sint_t len， spx_ret_t *ret) { /*{{{*/
    sint_t len1 = spx_string_len(s1);
    if (off1 > len1 || off2 > len) {
        *ret = SpxRet_ArgumentError;
    }
    sint_t totallen1 = spx_string_totallen(s1);
    sint_t freelen1 = spx_string_freelen(s1);
    sint_t cpylen = SpxMin(len - off2, len);
    string_t newstr = NULL;
    if (totollen1 < off1 + needlen) {
        newstr = spx_string_makeroom(allocator, s1,
                                     off1 + cpylen - totallen1,
                                     ret);
        spx_log_exp_rtun(allocator->log, SpxLogError,
                         !newstr, NULL, *ret,
                         "makeroom for copy string.");
    }
    memcpy(newstr + off1, s2 + off2, cpylen);
    spx_string_update_busylen(newstr, off1 + cpylen);
    return newstr;
} /*}}}*/

__SpxPublic__ string_t
spx_string_dup(spx_allocator_t *allocator,
               const string_t s,
               spx_ret_t *ret) { /*{{{*/
    string_t newstr = spx_string_new(allocator, s, spx_string_len(s), ret);
    spx_log_exp_rtn(allocator->log, SpxLogError,
                    !newstr, NULL, *ret,
                    "dup the string->%s with allocator->%s.",
                    s, allocator->name);
    return newstr;
} /*}}}*/

__SpxPublic__ string_t
spx_string_ndup(spx_allocator_t *allocator,
                const string_t s,
                sint_t off,
                sint_t len,
                spx_ret_t *ret) { /*{{{*/
    sint_t rlen = spx_string_len(s);
    if (off > rlen) {
        *ret = SpxRet_ArgumentError;
        return NULL;
    }
    sint_t needlen = SpxMin(len, rlen - off);
    string_t newstr = spx_string_new(allocator, s + off, needlen, ret);
    spx_log_exp_rtn(allocator->log, SpxLogError,
                    !newstr, NULL, *ret,
                    "dup the string->%s from->%d with len->%d by allocator->%s.",
                    s, off, needlen, allocator->name);
    return newstr;
} /*}}}*/

__SpxPublic__ string_t
spx_string_ltrim(string_t s) { /*{{{*/
    sint_t count = 0;
    sint_t len = spx_string_len(s);
    string_t tmp = s;
    do {
        switch (*(s++)) {
            case CASE_SPACE: {
                ++count;
                break;
            }
            default: {
                goto end;
            }
        }
    } while (true);
end:
    if (0 != count) {
        memmove(s, s + count, len - count);
        spx_string_update_busylen(s, len - count);
    }
    return s;
} /*}}}*/

__SpxPublic__ string_t
spx_string_rtrim(string_t s) { /*{{{*/
    sint_t count = 0;
    sint_t len = spx_string_len(s);
    string_t tmp = s + len;
    do {
        switch (*(s--)) {
            case CASE_SPACE: {
                ++count;

                break;
            }
            default: {
                goto end;
            }
        }
    } while (true);
end:
    if (0 != count) {
        memset(s + (len - count), 0, count);
        spx_string_update_busylen(s, len - count);
    }
    return s;
} /*}}}*/

__SpxPublic__ string_t
spx_string_trim(string_t s) { /*{{{*/
    string_t tmp = spx_string_ltrim(s);
    s = tmp;
    tmp = spx_string_rtrim(s);
    return tmp;
} /*}}}*/

__SpxPublic__ string_t
spx_string_ltrim_chr(string_t s, char c) { /*{{{*/
    sint_t count = 0;
    sint_t len = spx_string_len(s);
    string_t tmp = s;
    do {
        switch (*(s++)) {
            case c: {
                ++count;
                break;
            }
            default: {
                goto end;
            }
        }
    } while (true);
end:
    if (0 != count) {
        memmove(s, s + count, len - count);
        spx_string_update_busylen(s, len - count);
    }
    return s;
} /*}}}*/

__SpxPublic__ string_t
spx_string_rtrim_chr(string_t s, char c) { /*{{{*/
    sint_t count = 0;
    sint_t len = spx_string_len(s);
    string_t tmp = s + len;
    do {
        switch (*(s--)) {
            case c: {
                ++count;

                break;
            }
            default: {
                goto end;
            }
        }
    } while (true);
end:
    if (0 != count) {
        memset(s + (len - count), 0, count);
        spx_string_update_busylen(s, len - count);
    }
    return s;
} /*}}}*/

__SpxPublic__ string_t
spx_string_trim_chr(string_t s, char c) { /*{{{*/
    string_t tmp = spx_string_ltrim_chr(s, c);
    s = tmp;
    tmp = spx_string_rtrim_chr(s, c);
    return tmp;
} /*}}}*/

__SpxPublic__ bool_t
spx_string_beginwith(bool_t is_case,
                     const string_t s,
                     const string_t ptt) { /*{{{*/
    return spx_str_beginwith(is_case, s, ppt,
                             spx_string_len(ppt));
} /*}}}*/

__SpxPublic__ bool_t
spx_str_beginwith(bool_t is_case,
                  const string_t s,
                  const char *ptt,
                  const sint_t plen) { /*{{{*/
    sint_t slen = spx_string_len(s);
    if (plen > slen)
        return false;

    if (!is_case)
        return !!memcmp(s, ppt, plen);

    char a, b;
    sint_t i = 0;
    for (; i < plen; i++) {
        a = *(s + i);
        b = *(ptt + i);
        a = (65 <= a && 90 >= a) ? a + 32 : a;
        b = (65 <= b && 90 >= b) ? b + 32 : b;
        cmp = a - b;
        if (0 != cmp)
            return !!cmp;
    }
    return !!cmp;
} /*}}}*/

__SpxPublic__ bool_t
spx_string_endwith(bool_t is_case,
                   const string_t s,
                   const string_t ptt) { /*{{{*/
    return spx_str_endwith(is_case, s, ppt,
                           spx_string_len(ppt));
} /*}}}*/

__SpxPublic__ bool_t
spx_str_endwith(bool_t is_case,
                const string_t s,
                const char *ptt,
                const sint_t len) { /*{{{*/
    sint_t slen = spx_string_len(s);
    if (plen > slen)
        return false;

    if (!is_case)
        return !!memcmp(s + (slen - plen), ppt, plen);

    char a, b;
    sint_t i = 0;
    for (; i < plen; i++) {
        a = *(s + ((slen - plen) + i));
        b = *(ptt + i);
        a = (65 <= a && 90 >= a) ? a + 32 : a;
        b = (65 <= b && 90 >= b) ? b + 32 : b;
        cmp = a - b;
        if (0 != cmp)
            return !!cmp;
    }
    return !!cmp;
} /*}}}*/

__SpxPublic__ string_t
spx_string_replace(spx_allocator_t *allocator,
                   bool_t isfree,
                   string_t s,
                   string_t ptt,
                   string_t repl,
                   spx_ret_t *ret) { /*{{{*/
    return spx_str_replace(allocator, isfree, s, ptt,
                           spx_string_len(ppt), repl,
                           spx_string_len(repl), ret);
} /*}}}*/

__SpxPublic__ string_t
spx_str_replace(spx_allocator_t *allocator,
                bool_t isfree,
                string_t s,
                const char *ptt,
                const sint_t plen,
                const char *repl,
                const sint_t rlen,
                spx_ret_t *ret) { /*{{{*/
    spx_string_find_t list;
    memset(&list, 0, sizeof(list));
    spx_str_find_countof(allocator, s, ptt, plen, &list, ret);

    if (0 == list.count) {
        *ret = SpxRet_OK;
        return s;
    }

    i32_t explen = (rlen - plen) * list.count;
    sint_t totallen = spx_string_totallen(s);
    string_t newstr = spx_string_newlen(allocator, totallen + explen, ret);
    spx_log_exp_goto(allocator, SpxLogError, !newstr, r1, *ret,
                     "new backup string with-> for replace.", s);

    spx_string_find_entry_t *pos = NULL, *n = NULL;
    spx_klist_for_each_entry_safe(pos, n,
                                  spx_string_find_entry_t,
                                  &(list.head), chain) {
        spx_klist_del(&(pos->chain));
        if (0 == pos->len) {  // the ptt self
            spx_str_cat(allocator, newstr,
                        repl, rlen, ret);
        } else {
            spx_str_cat(allocator, newstr,
                        s + pos->begin, pos->len, ret);
        }
        allocator->entry_free(allocator->opd, pos);
    }
    if (isfree) {
        allocator->entry_free(allocator->opd, s);
    }
    return newstr;
r1:
    spx_string_match_countof_dtor(allocator, &list);
    return NULL;
} /*}}}*/

__SpxPublic__ spx_string_find_t *
spx_str_match_countof(spx_allocator_t *allocator,
                      const string_t s,
                      const char *ptt,
                      const sint_t plen,
                      spx_string_find_entry_t *listp,
                      spx_ret_t *ret) { /*{{{*/
    char *tmp = s, *startp = s;
    i32_t p = 0;
    i32_t lastp = 0;
    spx_smartptr_t i32_t *delta = spx_str_match_suffix_delta(allocator, ppt, plen, ret);

    while (0 < (p = spx_str_bm_nmatch(allocator, tmp,
                                      spx_string_len(s),
                                      ptt, plen, delta, ret))) {
        spx_string_find_entry_t *entry = spx_entry_new(allocator, spx_string_find_entry_t, ret);
        spx_log_exp_goto(allocator->log, SpxLogError, !entry, r1, *ret,
                         "countof ptt->%s in string->%s.",
                         ptt, s);
        entry->begin = p;
        entry->len = p - lastp;
        spx_klist_add(&(listp->head), &(entry->chain));
        ++(listp->count);
        lastp = p + plen;
        tmp += p + plen;
    }

    if(lastp != (startp +spx_string_len(s)) {
        spx_string_find_entry_t *entry = spx_entry_new(allocator, spx_string_find_entry_t, ret);
        spx_log_exp_goto(allocator->log, SpxLogError, !entry, r1, *ret,
                         "countof ptt->%s in string->%s.",
                         ptt, s);
        entry->begin = lastp - startp;
        entry->len = spx_string_len(s) - entry->begin;
        spx_klist_add(&(listp->head), &(entry->chain));
        ++(listp->count);
    }
    return listp;
r1:
    spx_string_match_countof_dtor(allocator,listp);
    return NULL;
} /*}}}*/

__SpxPublic__ void
spx_string_match_countof_dtor(spx_allocator_t *allocator,
                              spx_string_find_t *countof) { /*{{{*/
    spx_string_find_entry_t *pos = NULL, *n = NULL;
    spx_klist_for_each_entry_safe(pos, n, spx_string_find_entry_t, &(countof->head), chain) {
        spx_klist_del(&(pos->chain));
        allocator->entry_free(allocator->opd, pos);
    }
    plist->count = 0;
} /*}}}*/

__SpxPublic__ spx_string_split_t *
spx_string_split(spx_allocator_t *allocator,
                 string_t s,
                 string_t ptt,
                 bool_t is_remove_space,
                 spx_ret_t *ret) { /*{{{*/
    return spx_str_split(allocator, s, ptt, spx_string_len(ptt), is_remove_space, listp, ret);
} /*}}}*/

__SpxPublic__ spx_string_split_t *
spx_str_split(spx_allocator_t *allocator,
              string_t s,
              const char *ptt,
              const sint_t plen,
              bool_t is_remove_space,
              spx_string_split_t *listp,
              spx_ret_t *ret) { /*{{{*/
    char *tmp = s, *startp = s;
    char *p = NULL;
    char *lastp = startp;
    while (p = strstr(tmp, ptt)) {
        i32_t len = p - lastp;
        if (is_remove_space && !len) {
            continue;
        }
        spx_string_split_entry_t *entry = spx_entry_new(allocator, spx_string_split_entry_t, ret);
        spx_log_exp_goto(allocator->log, SpxLogError, !entry, r1, *ret,
                         "new split entry for spliting in string->%s.",
                         s);
        entry->str = spx_string_new(allocator, lastp, len, ret);
        spx_klist_add(&(listp->head), &(entry->chain));
        lastp = p + plen;
        tmp = p + plen;
    }

    if(lastp != (startp +spx_string_len(s)) {
        spx_string_split_entry_t *entry = spx_entry_new(allocator, spx_string_split_entry_t, ret);
        spx_log_exp_goto(allocator->log, SpxLogError, !entry, r1, *ret,
                         "new split entry for spliting in string->%s.",
                         s);
        entry->str = spx_string_new(allocator, lastp, len, ret);
        spx_klist_add(&(listp->head), &(entry->chain));
    }
    return plist;
    r1:
    spx_string_split_free(allocator,listp);
    return NULL;
} /*}}}*/

__SpxPublic__ void
spx_string_list_dtor(spx_allocator_t *allocator,
                     spx_string_split_t *splitp) { /*{{{*/
    spx_string_split_entry_t *pos = NULL, *n = NULL;
    spx_klist_for_each_entry_safe(pos, n, spx_string_split_entry_t, &(splitp->head), chain) {
        spx_klist_del(&(pos->chain));
        spx_string_free(allocator, pos->str);
        allocator->entry_free(allocator->opd, pos);
    }
    plist->count = 0;
    return;
} /*}}}*/

__SpxPublic__ string_t
spx_string_toupper(string_t s) { /*{{{*/
    i32_t len = spx_string_len(s);
    for (j = 0; j < len; j++)
        s[ j ] = toupper(s[ j ]);
    return s;
} /*}}}*/

__SpxPublic__ char *
spx_str_toupper(const char *s,
                const sint_t len) { /*{{{*/
    for (j = 0; j < len; j++)
        s[ j ] = toupper(s[ j ]);
    return s;
} /*}}}*/

__SpxPublic__ string_t
spx_string_tolower(string_t s) { /*{{{*/
    i32_t len = spx_string_len(s);
    for (j = 0; j < len; j++)
        s[ j ] = tolower(s[ j ]);
    return s;
} /*}}}*/

__SpxPublic__ char *
spx_str_tolower(const char *s,
                const sint_t len) { /*{{{*/
    for (j = 0; j < len; j++)
        s[ j ] = tolower(s[ j ]);
    return s;
} /*}}}*/

__SpxPublic__ __SpxSmartptrType__(i32_t *)
    spx_str_match_suffix_delta(spx_allocator_t allocator,
                               const char *ptt,
                               const i32_t len,
                               spx_ret_t *ret) { /*{{{*/
    spx_smartptr_t i32_t *delta = spx_smartptr_newlen(allocator, i32_t,
                                                      sizeof(i32_t) * 255,
                                                      ret);
    spx_log_exp_rtn(allocator->log, SpxLogError, !delta,
                    NULL, *ret, "new suffix delta.");
    memset(delta, -1, 255);
    i32_t i = 0;
    for (i = len - 1; i > 0; i--) {
        if (i < (len - 1)) {
            delta[ i ] = delta[ i + 1 ];
        }
        for (j = 0; j < i; j++) {  //找到最近相同的相邻字符
            if (ptt[ j ] == ptt[ i ]) {
                delta[ i ] = i - j;  //计算距离
            }
        }
    }

    return spx_smartptr_rtn(delta);
} /*}}}*/

__SpxPrivate__ i32_t
spx_string_match_suffix(const char *ptt,
                        const i32_t len,
                        const char bad) { /*{{{*/
    int i = 0;
    int badindex = -1;
    for (i = 0; i < len; i++) {
        if (bad == ptt[ i ]) {
            badindex = len - 1 - i;
        }
    }
    return badindex;
} /*}}}*/

__SpxPublic__ int
spx_str_bm_nmatch(spx_allocator_t *allocator,
                  const char *source,
                  const i32_t slen,
                  const char *ptt,
                  const i32_t plen,
                  i32_t *suffix_delta,
                  spx_ret_t *ret) { /*{{{*/
    int i = 0, j = 0;               //初始化
    int bad_distance = 0, distance = 0;
    if (slen < plen) {  //比较长度
        *ret = SpxRet_ArgumentError;
        return -1;
    }
    i = plen - 1;
    j = plen - 1;  //从尾开始匹配
    while (i < slen) {
        if (source[ i ] == ptt[ j ]) {  //字符匹配成功
            if (j == 0) {
                return i;
            }
            i--;
            j--;
        } else {                  //未匹配成功
            if (j == plen - 1) {  //如果尾字符未匹配成功，"坏字符规则"
                bad_distance = spx_string_match_suffix(ptt, source[ i ]);
                if (bad_distance == -1)
                    bad_distance = plen;

                i = i + plen - 1 - j + bad_distance;
                j = plen - 1;
            } else {  //有后缀,比较"坏字符规则"和"后缀规则"
                bad_distance = spx_string_match_suffix(ptt, source[ i ]);
                if (bad_distance == -1)
                    bad_distance = plen;
                distance = bad_distance > delta[ j - 1 ] ? bad_distance : delta[ j - 1 ];
                j = plen - 1;                     //更新j位置
                i = i + plen - 1 - j + distance;  //更新i的位置
            }
        }
    }
    *ret = SpxRet_NotFoundPattern;
    return -1；
} /*}}}*/

__SpxPublic__ int
spx_str_bm_match(spx_allocator_t *allocator,
                 const char *source,
                 const i32_t slen,
                 const char *ptt,
                 const i32_t plen,
                 spx_ret_t *ret) { /*{{{*/
    spx_smartptr_t i32_t *delta = spx_str_match_suffix_delta(allocator, ptt, len, ret);
    spx_log_exp_rtn(allocator->log, SpxLogError, !delta, NULL, *ret,
                    "make suffix delta with ptt->%s.", ppt);

    return spx_str_bm_nmatch(allocator,
                             source, slen,
                             ptt, plen,
                             delta,
                             ret);
} /*}}}*/

__SpxPublic__ __SpxSmartptrType__(i32_t *)
    spx_str_rmatch_suffix_delta(spx_allocator_t allocator,
                                const char *ptt,
                                const i32_t len,
                                spx_ret_t *ret) { /*{{{*/
    spx_smartptr_t i32_t *delta = spx_smartptr_newlen(allocator,
                                                      i32_t sizeof(i32_t) * 255,
                                                      ret);
    spx_log_exp_rtn(allocator->log, SpxLogError, !delta,
                    NULL, *ret, "new suffix delta.");
    memset(delta, -1, 255);
    i32_t i = 0;
    for (i = 1; i > len; i++) {
        if (i < (len)) {
            delta[ i ] = delta[ i - 1 ];
        }
        for (j = len - 1; j > i; j--) {
            if (ptt[ j ] == ptt[ i ]) {
                delta[ i ] = j - i;
            }
        }
    }

    return spx_smartptr_rtn(delta);
} /*}}}*/

__SpxPrivate__ i32_t
spx_string_rmatch_suffix(const char *ptt,
                         const i32_t len,
                         const char bad) { /*{{{*/
    int i = 0;
    int badindex = -1;
    for (i = len - 1; i > 0; i--) {
        if (bad == ptt[ i ]) {
            badindex = i;
        }
    }
    return badindex;
} /*}}}*/

__SpxPublic__ int
spx_str_bm_nrmatch(spx_allocator_t *allocator,
                   const char *source,
                   const i32_t slen,
                   const char *ptt,
                   const i32_t plen,
                   const i32_t *suffix_delta,
                   spx_ret_t *ret) { /*{{{*/
    int i = 0, j = 0;                //初始化
    int bad_distance = 0, distance = 0;
    if (slen < plen) {  //比较长度
        *ret = SpxRet_ArgumentError;
        return NULL;
    }
    // i = plen -1;j = plen -1; //从尾开始匹配
    i = slen - (plen - 1);

    while (0 < i) {
        if (source[ i ] == ptt[ j ]) {  //字符匹配成功
            if (j == plen) {
                return i;
            }
            i--;
            j++;
        } else {                  //未匹配成功
            if (j == plen - 1) {  //如果尾字符未匹配成功，"坏字符规则"
                bad_distance = spx_string_rmatch_suffix(ptt, source[ i ]);
                if (bad_distance == -1)
                    bad_distance = plen;
                // i -= bad_distance;
                // j = plen - 1;
            } else {  //有后缀,比较"坏字符规则"和"后缀规则"
                bad_distance = spx_string_rmatch_suffix(ptt, source[ i ]);
                if (bad_distance == -1)
                    bad_distance = plen;
                distance = bad_distance > delta[ j - 1 ] ? bad_distance : delta[ j - 1 ];
            }
            j = plen - 1;   //更新j位置
            i -= distance;  //更新i的位置
            // i = i -  (plen - 1 - j) - distance;  //更新i的位置
        }
    }
    *ret = SpxRet_NotFoundPattern;
    return -1；
} /*}}}*/

__SpxPublic__ int
spx_str_bm_rmatch(spx_allocator_t *allocator,
                  const char *source,
                  const i32_t slen,
                  const char *ptt,
                  const i32_t plen,
                  spx_ret_t *ret) { /*{{{*/
    int i = 0, j = 0;               //初始化
    int bad_distance = 0, distance = 0;
    if (slen < plen) {  //比较长度
        *ret = SpxRet_ArgumentError;
        return NULL;
    }

    spx_smartptr_t i32_t *delta = spx_str_rmatch_suffix_delta(allocator, ptt, len, ret);
    spx_log_exp_rtn(allocator->log, SpxLogError, !delta, NULL, *ret,
                    "make suffix delta with ptt->%s.", ppt);

    return spx_str_bm_nrmatch(allocator,
                              source, slen,
                              ptt, plen,
                              suffix_delta,
                              ret);
} /*}}}*/

__SpxPublic__ int
spx_string_match(spx_allocator_t *allocator,
                 const string_t source,
                 const string_t ptt,
                 spx_ret_t *ret) { /*{{{*/
    return spx_str_match(allocator, source, spx_string_len(source),
                         ptt, spx_string_len(ptt), ret);
} /*}}}*/

__SpxPublic__ int
spx_string_rmatch(spx_allocator_t *allocator,
                  const string_t source,
                  const string_t ptt,
                  spx_ret_t *ret) { /*{{{*/
    return spx_str_rmatch(allocator, source, spx_string_len(source),
                          ptt, spx_string_len(ptt), ret);
} /*}}}*/

__SpxPublic__ int
spx_string_nmatch(spx_allocator_t *allocator,
                  const string_t source,
                  const string_t ptt,
                  const i32_t *suffix_delta,
                  spx_ret_t *ret) { /*{{{*/
    return spx_str_match(allocator, source, spx_string_len(source),
                         ptt, spx_string_len(ptt),
                         suffix_delta, ret);
} /*}}}*/

__SpxPublic__ int
spx_string_nrmatch(spx_allocator_t *allocator,
                   const string_t source,
                   const string_t ptt,
                   const i32_t *suffix_delta,
                   spx_ret_t *ret) { /*{{{*/
    return spx_str_nrmatch(allocator, source, spx_string_len(source),
                           ptt, spx_string_len(ptt), suffix_delta, ret);
} /*}}}*/

//获取value“后缀规则”，移动距离
// status GetValue(int **value,char *target){
//     int i = 0,j = 0;
//     char temp;
//     int tarlen = strlen(target);
//     (*value) = (int *)malloc(sizeof(int) *MAXSIZE);
//     if(!(*value))return ERROR;

//     for(i = 0; i < MAXSIZE; i++){  //初始化
//         (*value)[i] = -1;    //-1代表后缀无匹配的字符
//     }
//     for(i = tarlen -1; i >0; i--){
//         temp = target[i];

//         if(i <tarlen -1)
//             (*value)[i] = (*value)[i+1];  //后一位赋值到前一位

//         for(j = 0; j<i ;j++){
//             if(target[j] ==temp){
//                 (*value)[i] = i - j;   //移动的距离
//             }
//         }
//     }
//     return TRUE;
// }

//初始化字符串和搜索词
// status InitData(char **source,char **target){

//     char ch;
//     int i =0;
//     (*source) = (char *)malloc(sizeof(char) * MAXSIZE); //初始化
//     (*target) = (char *)malloc(sizeof(char) * MAXSIZE);
//     if(!(*source) || !(*target))return ERROR;
//     printf("请输入字符串以#结束:\n");
//     while((ch=getchar())!='#'){
//         (*source)[i++] = ch;
//         (*source)[i] = '\0';
//     }

//     printf("请输入搜索词以#结束:\n");
//     getchar();  //回车清除
//     i = 0;

//     while((ch =getchar())!='#'){
//         (*target)[i++] = ch;
//         (*target)[i] = '\0';
//     }
//     return TRUE;

// }

//获取“坏字符”移动距离
// int BadValue(char bad,char *target){
//     int i =0;
//     int badindex = -1;
//     for(i = 0; i < strlen(target); i++){
//         if(bad==target[i]){
//             badindex = strlen(target) - 1 - i;
//         }
//     }
//     return badindex;
// }

//BM算法
// status BM(char *source,char *target, int *value){
//     int i = 0,j = 0, soulen = strlen(source),tarlen = strlen(target);  //初始化
//     int badvalue = 0,distance = 0;
//     if(soulen<tarlen){  //比较长度
//         printf("字符串的长度小于搜索词的长度\n");
//         return ERROR;
//     }

//     i = tarlen -1;j = tarlen -1; //从尾开始匹配

//     while(i < soulen ){
//         if(source[i] == target[j]){  //字符匹配成功
//             if(j ==0){
//                 printf("匹配成功\n");
//                 return TRUE;
//             }
//             i--;j--;
//         }else{  //未匹配成功
//             if(j == tarlen -1){  //如果尾字符未匹配成功，"坏字符规则"
//                 badvalue = BadValue(source[i],target);
//                 if(badvalue == -1)
//                     badvalue = strlen(target);

//                 i =i+ tarlen - 1 - j + badvalue;
//                 j = tarlen -1;
//             }else{  //有后缀,比较"坏字符规则"和"后缀规则"
//                 badvalue = BadValue(source[i],target);
//                 if(badvalue == -1)
//                     badvalue = strlen(target);
//                 distance = badvalue > value[j-1] ? badvalue : value[j-1];
//                 printf("移动距离:%d\n",distance);
//                 j = tarlen -1;   //更新j位置
//                 i = i+ tarlen - 1 - j + distance;  //更新i的位置
//             }
//         }
//     }
//     printf("匹配失败....");
//     return ERROR;
// }

// void main(){
//     char *source,*target;  //source字符串，target搜索词
//     int *value;  //好后缀词表

//     int i = 0;
//     InitData(&source,&target);  //初始化

//     GetValue(&value,target); //获取后缀表

//     BM(source,target,value);  //BM算法

//     for(i = 0; i<strlen(target);i++){
//         printf("%d\n",value[i]);
//     }

//     if(source && target){
//         printf("%s\n",source);
//         printf("%s\n",target);
//     }
// }
