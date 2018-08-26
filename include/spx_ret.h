#pragma once

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "_spx_ret_no.h"
#include "spx_ctype.h"
#include "spx_mfunc.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
        * Build an array of error strings
         *   e.g. {"Success","Invalid syntax","Out of range"}
          */
__SpxPrivate__ const char *SpxRetMsg[] = {
#define SR(Enum, Val, Msg) Msg,
    SpxRet
#undef SR
};

typedef enum {
#define SR(Enum, Val, Text) Enum = val,
    SpxRet
#undef SR
        SpxRet_MAX /*  Array size */
} spx_ret_t;

__SpxPrivate__ const char *
spx_ret_msg(spx_ret_t ret) {
    if (SpxRet_MAX <= ret) {
        return "no ret no";
    }

    return SpxRetMsg[ ret ];
}

    // have not useful,just for debug
    /*
     *    Build an array of error return values
     *     e.g. {0,5,8}
     */
    /*
__SpxPrivate__ int SpxRetNo[]  // SpxRet number
    = {
#define SR(Enum, No, Text) No,
          SpxRet
#undef SR
      };
*/
    /*
        Build an array of error enum names
        e.g. {"ERROR_NONE","ERROR_SYNTAX","ERROR_RANGE"}
         */
    /*
static char *SpxRetEnum[] = {
#define SR(Enum, Val, Text) #Enum,
    SpxRet
#undef SR
};
 */

    /* __SpxPrivate__ void showErrorInfo(void) {
       int i;
       */
    /*
     *       * Access the values
     *            */
    /*     for (i = 0; i < SpxRet_MAX; i++)
           printf(" %s == %d [%s]\n", SpxRetEnum[ i ], SpxRetNo[ i ], SpxRetMsg[ i ]);
           } */

    /*
     * Test validity of an error value
     *      case ERROR_SUCCESS:
     *      case ERROR_SYNTAX:
     *      case ERROR_RANGE:
     */

    // switch (value) {
    // #define SR(Enum, Val, Text) case Val:
    //     SpxRet
    // #undef SR
    //     printf("Error %d is ok\n", value);
    //     break;
    //     default:
    //         printf("Invalid error: %d\n", value);
    //         break;
    // }

#ifdef __cplusplus
}
#endif
