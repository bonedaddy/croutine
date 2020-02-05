#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "../src/croutine/croutine.c"
#include "../include/array_len/array_len.h"
#include <time.h>
#ifdef _WIN32 // conditionally import sleep
#include <Windows.h>
#else
#include <unistd.h>
#endif

// https://stackoverflow.com/questions/1202687/how-do-i-get-a-specific-range-of-numbers-from-rand
int random_number(int min, int max){
   return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

// a "demo" function
void *go_process_func(void *arg) {
    croutine_data *data = (croutine_data *)arg;
    int rnum = random_number(0, MAX_THREADS);
    printf("sleeping for %d seconds\n", rnum);
    sleep(rnum);
    // type cast pointer arg to pointer go_data struct
    printf("hello from go_process_func, thread id: %d\n", data->tid);
    pthread_exit(NULL);
}


int main() {
    pthread_t threads[MAX_THREADS];
    croutine_data threads_data[MAX_THREADS];
    croutine_future *futures[MAX_THREADS];
    for (int i = 0; i < MAX_THREADS; i++) {
        futures[i] = new_croutine_future();
    }
    for (int i = 0; i < array_len(futures); ++i) {
        int id = i + 1;
        threads_data[i].tid = id;
        fire_croutine_fut(
            &threads[i],
            NULL,
            &threads_data[i],
            go_process_func,
            futures[i]
        );
    }
    for (int i = 0; i < array_len(futures); ++i) {
        pthread_join(threads[i], NULL);
    }
    for (int i = 0; i < array_len(futures); ++i) {
        if (get_exit_code_croutine_fut(futures[i]) != 0) {
            perror("non zero exit code");
            exit((*futures[i]).exitCode);
        }
        if (!has_fired_croutine_fut(futures[i])) {
            perror("failed to fie futures");
            exit(1);
        }
    }
    printf("%s", "all futures fired\n");
    return 0;   
}