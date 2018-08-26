#pragma once

#ifdef __cplusplus
extern "C" {
#endif

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

typedef struct {
    spx_klist_node_t head;
    sint_t count;
} spx_string_split_t;

typedef struct {
    spx_klist_node_t chain;
    string_t str;
} spx_string_split_entry_t;

typedef struct {
    spx_klist_node_t head;
    sint_t count;
} spx_string_find_t;

typedef struct {
    spx_klist_node_t chain;
    i32_t begin;
    i32_t len;
} spx_string_find_entry_t;

/**
     * get the string context length,add the string end '\0'
     */
__SpxPrivate__ __SpxInline__ sint_t
spx_string_ctxlen(const sint_t len) { /*{{{*/
    return ++len;
} /*}}}*/

/*
     * get the string free length,the result not cover the string end '\0'
     */
__SpxPrivate__ __SpxInline__ sint_t
spx_string_freelen(const string_t s) { /*{{{*/
    spx_entry_t *str = spx_container_of(s, spx_entry_t, buf);
    return str->len - sizeof(spx_entry_t) - str->busylen;
} /*}}}*/

/*
     * get the string total length
     */
__SpxPrivate__ __SpxInline__ sint_t
spx_string_totallen(const string_t s) { /*{{{*/
    spx_entry_t *str = spx_container_of(s, spx_entry_t, buf);
    return str->len - sizeof(spx_entry_t);  // sub the string end '\0'
} /*}}}*/

/*
     * get the string length,not cover the string end '\0'
     */
__SpxPrivate__ __SpxInline__ sint_t
spx_string_len(const string_t s) { /*{{{*/
    spx_entry_t *str = spx_container_of(s, spx_entry_t, buf);
    return str->busylen - 1;  // not cover string end '\0'
} /*}}}*/

/*
     * update busylen in string metadata
     * busylen: must cover the string end '\0'
     */
__SpxPrivate__ __SpxInline__ void
spx_string_update_busylen(const string_t s,
                          const sint_t busylen) { /*{{{*/
    spx_entry_update_busylen(s, busylen);
    if (0 == busylen) {  //special
        str[ busylen ] = '\0';
    } else {
        str[ busylen - 1 ] = '\0';
    }
} /*}}}*/

/*
     * clear the string context
     */
__SpxPrivate__ __SpxInline__ void
spx_string_clear(string_t s) { /*{{{*/
    spx_entry_t *str = spx_container_of(s, spx_entry_t, buf);
    memset(s, 0, str->busylen);
    str->busylen = 0;
    return;
} /*}}}*/

__SpxPrivate__ __SpxInline__ bool_t
spx_string_is_null_or_empty(const string_t s) { /*{{{*/
    if (NULL == s)
        return true;
    spx_entry_t *str = spx_container_of(s, spx_entry_t, buf);
    return !(str->busylen);
} /*}}}*/

__SpxPrivate__ __SpxInline__ bool_t
spx_string_beginwith_chr(const string_t s, const char c) { /*{{{*/
    return !!(*s == c);
} /*}}}*/

__SpxPrivate__ __SpxInline__ bool_t
spx_string_endwith_chr(const string_t s, const char c) { /*{{{*/
    spx_entry_t *str = spx_container_of(s, spx_entry_t, buf);
    return !!(*(s + str->busylen) == c);
} /*}}}*/

__SpxPrivate__ __SpxInline__ bool_t
spx_string_exist_chr(const string_t s, const char c) { /*{{{*/
    return !!index(s, ((int) c));
} /*}}}*/

/*
     * release the string
     */
__SpxPrivate__ __SpxInline__ void
spx_string_free(spx_allocator_t *allocator,
                string_t *s) { /*{{{*/
    struct spx_string_t *str = spx_container_of(s, spx_string_t, buf);
    allocator->entry_free(allocator->opd, str);
} /*}}}*/

/*
     * new string with init by len.
     * if alloc fail,return NULL and ret specified error code
     */
__SpxPublic__ string_t
spx_string_new(spx_allocator_t *allocator,
               const void *init,
               const sint_t len,
               spx_ret_t *ret);

/*
     * new empty string with len.
     * if alloc fail,return NULL and ret specified error code
     */
__SpxPublic__ string_t
spx_string_newlen(spx_allocator_t *allocator,
                  const sint_t len,
                  spx_ret_t *ret);

__SpxPublic__ __SpxSmartptrType__(string_t)
    spx_string_smartptr(spx_allocator_t *allocator,
                        const void *init,
                        const sint_t len,
                        spx_ret_t *ret);

/*
     * cmp string and ptt
     * is_case:if is_case is true,cmp by ignore case
     * result: if s > ptt return greater than 0
     *          if s == ptt return 0
     *          if s < ptt return less than 0
     */
__SpxPublic__ int
spx_string_cmp(bool_t is_case,
               const string_t s,
               const string_t ptt);

/*
     * cmp string and ptt with len
     * is_case:if is_case is true,cmp by ignore case
     * result: if s > ptt return greater than 0
     *          if s == ptt return 0
     *          if s < ptt return less than 0
     */
__SpxPublic__ int
spx_str_cmp(bool_t is_case,
            const string_t s,
            const char *ptt,
            const sint_t len);

/*
     * append s2 to s1.
     * if free-length(s1) < legnth(s2),then,then expand s1.
     * so maybe s1 will be changed the address.
     * result: new string will returned,if fail,ret will be set fail-code.
     */
__SpxPublic__ string_t
spx_string_cat(spx_allocator_t *allocator,
               string_t s1,
               const string_t s2,
               spx_ret_t *ret);

/*
     * append s2 with len to s1.
     * if free-length(s1) < legnth(s2),then expand s1.
     * so maybe s1 will be changed the address.
     * result: new string will returned,if fail,ret will be set fail-code.
     */
__SpxPublic__ string_t
spx_str_cat(spx_allocator_t *allocator,
            string_t s1,
            const char *s2,
            const sint_t len,
            spx_ret_t *ret);

/*
     * padding append s2 with pad to s1.
     * if length(s2) < plen,then padding plen - length(s2) in the end with s2 by pad.
     * if free-length(s1) < legnth(s2),then expand s1.
     * so maybe s1 will be changed the address.
     * result: new string will returned,if fail,ret will be set fail-code.
     */
__SpxPublic__ string_t
spx_string_pcat(spx_allocator_t *allocator,
                string_t s1,
                const string_t s2,
                const sint_t plen,
                const char pad);

/*
     * padding append s2 with pad to s1.
     * if len < plen,then padding plen - len in the end with s2 by pad.
     * if free-length(s1) < leg,then expand s1.
     * so maybe s1 will be changed the address.
     * result: new string will returned,if fail,ret will be set fail-code.
     */
__SpxPublic__ string_t
spx_str_pcat(spx_allocator_t *allocator,
             string_t s1,
             const char *s2,
             const sint_t len,
             const sint_t plen,
             const char pad);

/*
     *append format string  to s1.
     * if free-length(s1) < length(format-string),then expand s1.
     * so maybe s1 will be changed the address.
     * result: new string will returned,if fail,ret will be set fail-code.
     */
__SpxPublic__ string_t
spx_string_nvcat(spx_ret_t *ret,
                 spx_allocator_t *allocator,
                 string_t s,
                 const char *fmt,
                 va_list ap);

/*
     *append format string  to s1.
     * if free-length(s1) < length(format-string),then expand s1.
     * so maybe s1 will be changed the address.
     * result: new string will returned,if fail,ret will be set fail-code.
     */
__SpxPublic__ string_t
spx_string_vcat(spx_ret_t *ret,
                spx_allocator_t *allocator,
                string_t s,
                const char *fmt,
                ...);

/*
     * copy s2 begin off2 to len to s1 offset off1.
     * if free-length(s1) < copy length,then expand s1.
     * so maybe s1 will be changed the address.
     * result: new string will returned,if fail,ret will be set fail-code.
     */
__SpxPublic__ string_t
spx_string_cpy(spx_allocator_t *allocator,
               string_t s1,
               sint_t off1,
               const string_t s2,
               const sint_t off2,
               const sint_t len,
               spx_ret_t *ret);

/*
     * copy s2 from off2 with len to s1 offset off1.
     * if free-length(s1) < copy length,then expand s1.
     * so maybe s1 will be changed the address.
     * result: new string will returned,if fail,ret will be set fail-code.
     */
__SpxPublic__ string_t
spx_str_cpy(spx_allocator_t *allocator,
            string_t s1,
            const sint_t off1,
            const char *s2,
            const sint_t off2,
            const sint_t len， spx_ret_t *ret);

/*
     * duplicate a new  string by s
     * result: new string will returned,if fail,ret will be set fail-code.
     */
__SpxPublic__ string_t
spx_string_dup(spx_allocator_t *allocator,
               const string_t s,
               spx_ret_t *ret);

/*
     * duplicate a new  string by s from off with len
     * result: new string will returned,if fail,ret will be set fail-code.
     */
__SpxPublic__ string_t
spx_string_ndup(spx_allocator_t *allocator,
                const string_t s,
                sint_t off,
                sint_t len,
                spx_ret_t *ret);

/*
     * trim space symbol at the begin of s
     */
__SpxPublic__ string_t
spx_string_ltrim(string_t s);

/*
     * trim space symbol at the end of s
     */
__SpxPublic__ string_t
spx_string_rtrim(string_t s);

/*
     * trim space symbol at both ends of s
     */
__SpxPublic__ string_t
spx_string_trim(string_t s);

/*
     * trim symbol c at the begin of s
     */
__SpxPublic__ string_t
spx_string_ltrim_chr(string_t s, char c);

/*
     * trim symbol c at the end of s
     */
__SpxPublic__ string_t
spx_string_rtrim_chr(string_t s, char c);

/*
     * trim symbol c at both ends of s
     */
__SpxPublic__ string_t
spx_string_trim_chr(string_t s, char c);

/*
     * determine s whether to start with ptt，and if is_case is true,then not case sensitive.
     * result : true , start with ptt
     *          false,start without ptt
     */
__SpxPublic__ bool_t
spx_string_beginwith(bool_t is_case,
                     const string_t s,
                     const string_t ptt);

/*
     * determine s whether to start with ptt，and if is_case is true,then not case sensitive.
     * result : true , start with ptt
     *          false,start without ptt
     */
__SpxPublic__ bool_t
spx_str_beginwith(bool_t is_case,
                  const string_t s,
                  const char *ptt,
                  const sint_t plen);

/*
     * determine s whether to end with ptt，and if is_case is true,then not case sensitive.
     * result : true , end with ptt
     *          false,start end ptt
     */
__SpxPublic__ bool_t
spx_string_endwith(bool_t is_case,
                   const string_t s,
                   const string_t ptt);

/*
     * determine s whether to end with ptt，and if is_case is true,then not case sensitive.
     * result : true , end with ptt
     *          false,start end ptt
     */
__SpxPublic__ bool_t
spx_str_endwith(bool_t is_case,
                const string_t s,
                const char *ptt,
                const sint_t len);

/*
     * replace ptt with repl in s
     * replace string is a new string,so if isfree== true,then free the s
     * result : return the new replaced string
     */
__SpxPublic__ string_t
spx_string_replace(spx_allocator_t *allocator,
                   bool_t isfree,
                   string_t s,
                   string_t ptt,
                   string_t repl,
                   spx_ret_t *ret);

/*
     * replace ptt with repl in s
     * replace string is a new string,so if isfree== true,then free the s
     * result : return the new replaced string
     */
__SpxPublic__ string_t
spx_str_replace(spx_allocator_t *allocator,
                bool_t isfree,
                string_t s,
                const char *ptt,
                const sint_t plen,
                const char *repl,
                const sint_t rlen,
                spx_ret_t *ret);

/*
     * Get the count of times ptt appears in s
     */
__SpxPublic__ spx_string_find_entry_t *
spx_str_match_countof(spx_allocator_t *allocator,
                      const string_t s,
                      const char *ptt,
                      const sint_t plen,
                      spx_string_find_entry_t *listp,
                      spx_ret_t *ret);

/*
     * destory countof struct
     * with type spx_string_match_countof_entry_t
     * from function spx_str_match_countof
     */
__SpxPublic__ void
spx_string_match_countof_dtor(spx_allocator_t *allocator,
                              spx_string_match_countof_t *countof);

/*
     * split s with ppt
     * if is_remove_space is true,remove the blank in split array
     * else keep blank in split array
     */
__SpxPublic__ spx_string_split_t *
spx_string_split(spx_allocator_t *allocator,
                 string_t s,
                 string_t ptt,
                 bool_t is_remove_space,
                 spx_ret_t *ret);

/*
     * split s with ppt
     * if is_remove_space is true,remove the blank in split array
     * else keep blank in split array
     */
__SpxPublic__ spx_string_split_t *
spx_str_split(spx_allocator_t *allocator,
              string_t s,
              const char *ptt,
              const sint_t plen,
              bool_t is_remove_space,
              spx_string_split_t *listp,
              spx_ret_t *ret);

/*
     * deatory split struct
     * with type spx_string_split_t
     * from function spx_string_split or spx_str_split
     */
__SpxPublic__ void
spx_string_list_dtor(spx_allocator_t *allocator,
                     spx_string_split_t *splitp);

/*
     * set all char to upper in string s
     */
__SpxPublic__ string_t
spx_string_toupper(string_t s);

/*
     * set all char to upper in char array s
     */
__SpxPublic__ char *
spx_str_toupper(const char *s, const sint_t len);

/*
     * set all char to lower in string s
     */
__SpxPublic__ string_t
spx_string_tolower(string_t s);

/*
     * set all char to lower in char array s
     */
__SpxPublic__ char *
spx_str_tolower(const char *s, const sint_t len);

/*
     * get suffix delta with ptt for string match
     * MARK:result is the smart pointer,so must use spx_smartptr_t define result var
     */
__SpxPublic__ __SpxSmartptrType__(i32_t *)
    spx_str_match_suffix_delta(spx_allocator_t allocator,
                               const char *ptt,
                               const i32_t len,
                               spx_ret_t *ret);
/*
     *  match ptt in source with suffix delta(from function spx_str_match_suffix_delta)
     *  the function can call more times by moving source with pattern
     * result:if matched return offset in the source
     *          else return -1
     *          if result is -1,then fail in function and result by ret
     */
__SpxPublic__ int
spx_str_bm_nmatch(spx_allocator_t *allocator,
                  const char *source,
                  const i32_t slen,
                  const char *ptt,
                  const i32_t plen,
                  i32_t *suffix_delta,
                  spx_ret_t *ret);
/*
     * match ptt in source with suffix delta(from function spx_str_match_suffix_delta)
     * result:if matched return offset in the source
     *          else return -1
     *          if result is -1,then fail in function and result by ret
     */
__SpxPublic__ int
spx_str_bm_match(spx_allocator_t *allocator,
                 const char *source,
                 const i32_t slen,
                 const char *ptt,
                 const i32_t plen,
                 spx_ret_t *ret);
/*
     * get suffix delta with ptt for string right match
     * MARK:result is the smart pointer,so must use spx_smartptr_t define result var
     */
__SpxPublic__ __SpxSmartptrType__(i32_t *)
    spx_str_rmatch_suffix_delta(spx_allocator_t allocator,
                                const char *ptt,
                                const i32_t len,
                                spx_ret_t *ret);
/*
     *  right match ptt in source with suffix delta(from function spx_str_match_suffix_delta)
     *  the function can call more times by moving source with pattern
     * result:if matched return offset in the source
     *          else return -1
     *          if result is -1,then fail in function and result by ret
     */
__SpxPublic__ int
spx_str_bm_nrmatch(spx_allocator_t *allocator,
                   const char *source,
                   const i32_t slen,
                   const char *ptt,
                   const i32_t plen,
                   const i32_t *suffix_delta,
                   spx_ret_t *ret);
/*
     * right match ptt in source with suffix delta(from function spx_str_match_suffix_delta)
     * result:if matched return offset in the source
     *          else return -1
     *          if result is -1,then fail in function and result by ret
     */
__SpxPublic__ int
spx_str_bm_rmatch(spx_allocator_t *allocator,
                  const char *source,
                  const i32_t slen,
                  const char *ptt,
                  const i32_t plen,
                  spx_ret_t *ret);
/*
     * match ptt in source with suffix delta(from function spx_str_match_suffix_delta)
     * result:if matched return offset in the source
     *          else return -1
     *          if result is -1,then fail in function and result by ret
     */
__SpxPublic__ int
spx_string_match(spx_allocator_t *allocator,
                 const string_t source,
                 const string_t ptt,
                 spx_ret_t *ret);
/*
     * right match ptt in source with suffix delta(from function spx_str_match_suffix_delta)
     * result:if matched return offset in the source
     *          else return -1
     *          if result is -1,then fail in function and result by ret
     */
__SpxPublic__ int
spx_string_rmatch(spx_allocator_t *allocator,
                  const string_t source,
                  const string_t ptt,
                  spx_ret_t *ret);

/*
     *  match ptt in source with suffix delta(from function spx_str_match_suffix_delta)
     *  the function can call more times by moving source with pattern
     * result:if matched return offset in the source
     *          else return -1
     *          if result is -1,then fail in function and result by ret
     */
__SpxPublic__ int
spx_string_nmatch(spx_allocator_t *allocator,
                  const string_t source,
                  const string_t ptt,
                  const i32_t *suffix_delta,
                  spx_ret_t *ret);
/*
     *  right match ptt in source with suffix delta(from function spx_str_match_suffix_delta)
     *  the function can call more times by moving source with pattern
     * result:if matched return offset in the source
     *          else return -1
     *          if result is -1,then fail in function and result by ret
     */
__SpxPublic__ int
spx_string_nrmatch(spx_allocator_t *allocator,
                   const string_t source,
                   const string_t ptt,
                   const i32_t *suffix_delta,
                   spx_ret_t *ret);

#ifdef __cplusplus
}
#endif
