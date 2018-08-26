#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>


#include "spx_ret.h"
#include "spx_mfunc.h"
#include "spx_klist.h"
#include "spx_spinlock.h"


spx_klist_node_t head;

typedef struct {
    spx_klist_node_t head;
    spx_spinlock_t *spin;
}spx_spinlock_test_t;

typedef struct {
    spx_klist_node_t head;
    pthread_mutex_t mutex;
}spx_mutexlock_test_t;

typedef struct {
    spx_klist_node_t chain;
    int tid ;
    int i;
}spx_freelock_test_t;

static int times = 0;

static pthread_t spx_detach_thread_new(
        i64_t stacksize,
        void *(*start_routine)(void*),
        void *arg,
        spx_ret_t *ret){/*{{{*/
    pthread_t tid = 0;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    size_t ostack_size = 0;
    pthread_attr_getstacksize(&attr, &ostack_size);
    do{
        if (((i64_t) ostack_size) != stacksize
                && (0 != (*ret = pthread_attr_setstacksize(&attr,stacksize)))){
            pthread_attr_destroy(&attr);
            break;
        }

        // if(0 != (*ret = pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED))){
        //     pthread_attr_destroy(&attr);
        //     break;
        // }
        if (0 !=(*ret =  pthread_create(&(tid), &attr, start_routine,
                        arg))){
            pthread_attr_destroy(&attr);
            break;
        }
    }while(false);
    pthread_attr_destroy(&attr);
    return tid;
}/*}}}*/



static void *thread_test_freelock(void *arg) {/*{{{*/
    int tid = *(int *)arg;
    int idx = 0;
    for(; idx < 100000;++idx) {
        spx_freelock_test_t *new = calloc(sizeof(spx_freelock_test_t),1);
        new->i =  tid * 1000 + idx;
        spx_kfreelist_add(&(new->chain),&head);
    }
    return NULL;
}/*}}}*/


static void *thread_test_spinlock(void *arg) {/*{{{*/
    spx_spinlock_test_t *st = (spx_spinlock_test_t *) arg;
    int idx = 0;
    spx_ret_t ret = 0;
    u64_t tid = pthread_self();
    for(; idx < 100000;++idx) {
        spx_freelock_test_t *new = calloc(sizeof(spx_freelock_test_t),1);
        new->tid = tid;
        new->i = idx;
        if(st->spin->lock(st->spin,tid,3,20,&ret)) {
            spx_klist_add(&(new->chain),&(st->head));
            times++;
            st->spin->freelock(st->spin,tid,&ret);
        }
    }
    return NULL;
}/*}}}*/

static void *thread_test_mutexlock(void *arg) {/*{{{*/
    spx_mutexlock_test_t *st = (spx_mutexlock_test_t *) arg;
    int idx = 0;
    spx_ret_t ret = 0;
    u64_t tid = pthread_self();
    for(; idx < 100000 && 0 < times;++idx) {
        spx_freelock_test_t *new = calloc(sizeof(spx_freelock_test_t),1);
        new->tid = tid;
        new->i = idx;
        pthread_mutex_lock(&(st->mutex));
        spx_klist_add(&(new->chain),&(st->head));
        times--;
        pthread_mutex_unlock(&(st->mutex));
    }
    return NULL;
}/*}}}*/

int main(int argc,char **argv) {
    spx_klist_init_head(&head);
    spx_ret_t ret = 0;
    spx_spinlock_test_t st;
    st.spin = spx_spinlock_new("freelist_locker",&ret);
    spx_klist_init_head(&(st.head));

    spx_mutexlock_test_t mt;
    pthread_mutex_init(&(mt.mutex),NULL);
    spx_klist_init_head(&(mt.head));


    clock_t start_t, end_t, total_t;
    struct timeval start_tv,end_tv;
    int  i = 0;
    pthread_t tids[10] ;
    start_t = clock();
    gettimeofday(&start_tv,NULL);
    for( ; i < 10;i++) {
        pthread_t tid = spx_detach_thread_new(4 * 1024 * 1024,thread_test_spinlock,&st,&ret);
        tids[i] = tid;
    }

    for(i = 0; i < 10; i++) {
        pthread_join(tids[i],NULL);
    }
    gettimeofday(&end_tv,NULL);
    end_t = clock();

    // spx_freelock_test_t *pos = NULL;
    // spx_freelock_test_t *n = NULL;
    // i = 0;
    // spx_klist_for_each_entry_safe(pos, n, spx_freelock_test_t, &(st.head), chain) {
    //     printf("%u -> %d \t ",pos->tid,pos->i);
    //     if(0 == ++i % 5) {
    //         printf("\n");
    //     }
    // }

    printf("spinlock times -> %d \n",times);
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("Total time taken by CPU: %f \n",(double) total_t  );
    printf("do exec time: sec -> %d,usec -> %d. \n",
           (int) (end_tv.tv_sec - start_tv.tv_sec),
           (int) (end_tv.tv_usec - start_tv.tv_usec));


    start_t = clock();
    gettimeofday(&start_tv,NULL);
    i = 0;
    for( ; i < times;i++) {
        spx_freelock_test_t *new = calloc(sizeof(spx_freelock_test_t),1);
        new->tid = 0;
        new->i = i;
        spx_klist_add(&(new->chain),&(head));
    }

    gettimeofday(&end_tv,NULL);
    end_t = clock();

    printf("nolock times -> %d \n",times);
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("Total time taken by CPU: %f \n",(double) total_t  );
    printf("do exec time: sec -> %d,usec -> %d. \n",
           (int) (end_tv.tv_sec - start_tv.tv_sec),
           (int) (end_tv.tv_usec - start_tv.tv_usec));


    pthread_t mids[10] ;
    start_t = clock();
    gettimeofday(&start_tv,NULL);
    i = 0;
    for( ; i < 10;i++) {
        pthread_t tid = spx_detach_thread_new(4 * 1024 * 1024,thread_test_mutexlock,&mt,&ret);
        mids[i] = tid;
    }

    for(i = 0; i < 10; i++) {
        pthread_join(mids[i],NULL);
    }
    gettimeofday(&end_tv,NULL);
    end_t = clock();

    printf("mutex times -> %d \n",times);
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("Total time taken by CPU: %f \n",(double) total_t  );
    printf("do exec time: sec -> %d,usec -> %d. \n",
           (int) (end_tv.tv_sec - start_tv.tv_sec),
           (int) (end_tv.tv_usec - start_tv.tv_usec));


    return 0;
}
