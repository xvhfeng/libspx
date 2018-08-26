#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "spx_ctype.h"
#include "spx_ret.h"

#ifndef spx_log_fn
    typedef void (spx_log_fn)(int level,string_t fmt,...);
#endif

#define SpxLogDebug 0
#define SpxLogInfo 1
#define SpxLogWarn 2
#define SpxLogError 3
#define SpxLogMark 4


__SpxPrivate__  char * SpxLogDesc[] = {
    "Debug.",
    "Info.",
    "Warn.",
    "Error.",
    "Mark."
};

 __SpxPrivate__  int SpxLogDescSize[] = {
    6,
    5,
    5,
    6,
    5
};


#define SpxLogLabel(log,level) \
    if(NULL != (log)) {\
        (log)(level,((string_t) "File:%s,Line:%d,Func:%s.this code is runned."), \
                __FILE__,__LINE__,__FUNCTION__); \
    }


#define spx_log(log,level,info) \
    if(NULL != (log)) {\
        (log)(level,((string_t) "File:%s,Line:%d,Func:%s.%s."), \
                __FILE__,__LINE__,__FUNCTION__,info); \
    }

#define spx_log_err(log,level,ret,info) \
    if(NULL != (log)) {\
        (log)(level,((string_t) "File:%s,Line:%d,Func:%s.errno:%d,info:%s.%s."),\
                __FILE__,__LINE__,__FUNCTION__,err,spx_ret_msg(ret),info);\
    }

#define spx_log_fmt(log,level,fmt,...) \
    if(NULL != (log)) {\
        (log)(level,((string_t) "File:%s,Line:%d,Func:%s."fmt),\
                __FILE__,__LINE__,__FUNCTION__,__VA_ARGS__);\
    }

#define spx_log_err_fmt(log,level,ret,fmt,...) \
    if(NULL != (log)) {\
        (log)(level,((string_t) "File:%s,Line:%d,Func:%s.errno:%d,info:%s."fmt),\
                __FILE__,__LINE__,__FUNCTION__,err,spx_ret_msg(ret),__VA_ARGS__);\
    }

    #define spx_log_exp(log,level,exp,ret,fmt,...) \
    if((exp) && (log)) { \
            (log)(level,((string_t) "File:%s,Line:%d,Func:%s.exp:%s,errno:%d,info:%s."fmt),\
                __FILE__,__LINE__,__FUNCTION__,(exp),(ret),spx_ret_msg(ret),__VA_ARGS__);\
    }

    #define spx_log_exp_rtn(log,level,exp,rtn,ret,fmt,...) \
    if(exp) { \
        if(log) { \
            (log)(level,((string_t) "File:%s,Line:%d,Func:%s.exp:%s,errno:%d,info:%s."fmt),\
                __FILE__,__LINE__,__FUNCTION__,(exp),(ret),spx_ret_msg(ret),__VA_ARGS__);\
        } \
        return (rtn); \
    }

    #define spx_log_exp_goto(log,level,exp,lable,ret,fmt,...) \
    if(exp) { \
        if(log)  { \
            (log)(level,((string_t) "File:%s,Line:%d,Func:%s.exp:%s,errno:%d,info:%s."fmt),\
                __FILE__,__LINE__,__FUNCTION__,(exp),(ret),spx_ret_msg(ret),__VA_ARGS__);\
        } \
        goto (lable); \
    }

#ifdef __cplusplus
extern "C" }
#endif