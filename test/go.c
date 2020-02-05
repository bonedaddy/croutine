#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "../src/go/go.c"
#include <time.h>
#ifdef _WIN32 // conditionally import sleep
#include <Windows.h>
#else
#include <unistd.h>
#endif

// a "demo" function
void *go_process_func(void *arg) {
    // type cast pointer arg to pointer go_data struct
    go_data *data = (go_data *)arg;
    sleep((*data).tid);
    printf("hello from go_process_func, thread id: %d\n", data->tid);
    pthread_exit(NULL);
}


int main() {
    pthread_t threads[MAX_THREADS];
    go_data threads_data[MAX_THREADS];
    struct go_goroutine_future *futures[MAX_THREADS];
    for (int i = 0; i < MAX_THREADS; i++) {
        futures[i] = new_go_goroutine_future();
    }
    for (int i = 0; i < MAX_THREADS; ++i) {
        int id = i + 1;
        threads_data[i].tid = id;
        fire_go_goroutine_fut(
            &threads[i],
            NULL,
            &threads_data[i],
            go_process_func,
            futures[i]
        );
    }
    for (int i = 0; i < MAX_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }
    for (int i = 0; i < MAX_THREADS; ++i) {
        if (get_exit_code_go_goroutine_fut(futures[i]) != 0) {
            perror("non zero exit code");
            exit((*futures[i]).exitCode);
        }
        if (!has_fired_go_goroutine_fut(futures[i])) {
            perror("failed to fie futures");
            exit(1);
        }
    }
    printf("%s", "all futures fired\n");
    return 0;   
}