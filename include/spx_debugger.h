#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#ifdef SpxDEBUG
#define spx_assert( s ) assert( s )
#define spx_assert_log( exp,log,level,fmt, ... )      \
    while(!(exp)) { \
        log(level,fmt, __VA_ARGS__ ); \
        assert( exp );              \
        break; \
    }
#else
#define spx_assert( s )
#define spx_assert_log( exp,log,level,fmt, ... ) printf( fmt, __VA_ARGS__ )
#endif


#ifdef SpxDEBUG
#define spx_probe( p, section )                                \
    struct timeval p##_start, p##_end;                        \
    gettimeofday( &( p##_start ), NULL );                     \
    section;                                                  \
    gettimeofday( &( p##_end ), NULL );                       \
    printf( "File:%s,Func:%s,Line:%d.run times(usec):%ld.\n", \
            __FILE__, __FUNCTION__, __LINE__,                 \
            (long) ( p##_end.tv_usec - p##_start.tv_usec ) )
#else
#define SpxProbe( p, section )
#endif

#ifdef __cplusplus
}
#endif