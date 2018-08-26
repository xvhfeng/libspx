#include <grp.h>
#include <pwd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "spx_allocator.h"
#include "spx_ctype.h"
#include "spx_log.h"
#include "spx_mfunc.h"
#include "spx_ret.h"
#include "spx_string.h"

void spx_env_daemon( ) { /*{{{*/
    pid_t pid;

    //no cmd
    if ((pid = fork( )) != 0) {
        exit(0);
    }

    //sid == pid and pgid == pid
    setsid( );

    //no open cmd again and cpid != sid
    if ((pid = fork( )) != 0) {
        exit(0);
    }
    return;
} /*}}}*/

/*
 * open detach child process to work
 * and make sure child process not to Zombie process
 * remark :
 *      this function will set father process SIGCLD to SIG_IGN
 *      or set SA_NOCLDWAIT
 * */
__SpxPublic__ spx_ret_t
spx_env_open_worker_process_detach(const i32_t worker_count,
                                   spx_worker_process_main_fu *workerp_main,
                                   i32_t *do_worker_count) { /*{{{*/
    spx_ret_t ret = SpxRet_OK;
    //father not accept child exit
    spx_env_sigaction(SIG_CHLD, SA_NOCLDWAIT, NULL);
    i32_t worker_num = worker_count;
    pid_t pid;
    while (worker_num > 0) {
        spx_errno_clear;
        pid = fork( );
        if (pid < 0) {
            ret = errno;
            break;
        } else if (pid == 0) {  // child
            workerp_main( );
        } else if (pid > 0) {  // father
            worker_num--;
        }
    }
    *do_worker_count = worker_count - worker_num;
    return ret;
} /*}}}*/
/*
 * open child process to work
 * and this function will blocking
 */
__SpxPublic__ spx_ret_t
spx_env_open_worker_process(const i32_t worker_count,
                            spx_worker_process_main_fn *workerp_main,
                            pid_t *pids) { /*{{{*/
    int status;
    i32_t worker_num = worker_count;
    pid_t pid;
    while (1) {
        if (worker_num > 0) {
            spx_errno_clear;
            pid = fork( );
            if (pid < 0) {
                perror("fork");
                exit(1);
            } else if (pid == 0) {  // child
                workerp_main( );
            } else if (pid > 0) {  // father
                worker_num--;
            }
        } else {
            // wait for a child to exit, and update worker_num
            pid = wait(&status);
            //printf("father: chile [%5d] is done. Create a new one...\n", pid);
            worker_num++;
        }
    }
    return SpxRet_OK;
} /*}}}*/

__SpxPublic__ void
spx_env_sigaction(const int sig,
                  const int flag,
                  const SpxSigActionDelegate *act) { /*{{{*/
    struct sigaction sa;
    SpxZero(sa);
    if (NULL == act) {
        sa.sa_handler = SIG_IGN;
    } else {
        sa.sa_handler = act;
    }
    sigemptyset(&(sa.sa_mask));
    sa.sa_flags = flag ?: SA_RESTART;
    sigaction(sig, &sa, NULL);
} /*}}}*/
