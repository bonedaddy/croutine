#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// STRUCTS

typedef struct go_thread_data {
    int tid;
    char *data;
} go_data;

typedef struct go_goroutine_future{
    char *returnValue;
    int exitCode;
    bool ready, fired;
    pthread_mutex_t mutex;
}  go_goroutine_future;

// FUNCTIONS

struct go_goroutine_future *new_go_goroutine_future();

void fire_go_goroutine_fut(
    pthread_t *thread, 
    pthread_attr_t *attr,
    void *arg, 
    void *(execute_func)(void *),
    struct go_goroutine_future *fut
);
void lock_go_goroutine_fut(struct go_goroutine_future *fut);
void unlock_go_goroutine_fut(struct go_goroutine_future *fut);
bool has_fired_go_goroutine_fut(struct go_goroutine_future *fut);
int get_exit_code_go_goroutine_fut(struct go_goroutine_future *fut);
