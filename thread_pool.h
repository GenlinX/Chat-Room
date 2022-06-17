#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#define MAX_WAITING_TASKS 1000
#define MAX_ACTIVE_THREADS 20

struct task //任务节点
{
    void *(*task)(void *arg);
    void *arg;
    struct task *next;
};

typedef struct thread_pool //线程池
{
    pthread_mutex_t lock;
    pthread_cond_t cond;
    struct task *task_list;

    pthread_t*tids;

    unsigned waiting_tasks;
    unsigned active_threads;

    bool shutdown;
} thread_pool;

//初始化线程池
bool init_pool(thread_pool *pool, unsigned int thread_number);

//投放任务
bool add_task(thread_pool *pool, void *(*task)(void *arg), void *arg);

//增加线程
int add_thread(thread_pool *pool, unsigned int additional_threads_number);

//删除线程
int remove_thread(thread_pool *pool, unsigned int removing_threads_number);

//销毁线程池
bool destroy_pool(thread_pool *pool);

//线程例程
void *routine(void *arg);

//运行线程
bool run_thread(thread_pool *pool);

#endif
