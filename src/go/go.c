#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "../../include/go/go.h"
#include <time.h>
#ifdef _WIN32 // conditionally import sleep
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define MAX_THREADS 4

void lock_go_goroutine_fut(struct go_goroutine_future *fut) {
    pthread_mutex_lock(&fut->mutex);
}
void unlock_go_goroutine_fut(struct go_goroutine_future *fut) {
    pthread_mutex_unlock(&fut->mutex);
}

bool has_fired_go_goroutine_fut(struct go_goroutine_future *fut) {
    lock_go_goroutine_fut(fut);
    bool fired = (*fut).fired;
    unlock_go_goroutine_fut(fut);
    return fired;
}

int get_exit_code_go_goroutine_fut(struct go_goroutine_future *fut) {
    lock_go_goroutine_fut(fut);
    int exitCode = (*fut).exitCode;
    unlock_go_goroutine_fut(fut);
    return exitCode;
}

/* 
fire_go_goroutine_fut: is used to execute an arbitrary function, storing the result in the "future"
This is my "hacky" attempt at a hybrid goroutine+channel
*/
void fire_go_goroutine_fut(
    pthread_t *thread, 
    pthread_attr_t *attr,
    void *arg, 
    void *(execute_func)(void *),
    struct go_goroutine_future *fut
) {
    int response = pthread_create(thread, attr, execute_func, arg);
    lock_go_goroutine_fut(fut);
    fut->exitCode = response;
    fut->returnValue = (char*)&response;
    fut->fired = true;
    unlock_go_goroutine_fut(fut);
}

struct go_goroutine_future *new_go_goroutine_future() {
    char *returnValue = malloc(sizeof(char));
    int exitCode;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    exitCode = 1;
    struct go_goroutine_future *fut = malloc(sizeof(go_goroutine_future));
    (*fut).returnValue = returnValue;
    (*fut).exitCode = exitCode;
    (*fut).mutex = mutex;
    (*fut).ready = true;
    return fut;
}

