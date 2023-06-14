#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "common_threads.h"

//
// Here, you have to write (almost) ALL the code. Oh no!
// How can you show that a thread does not starve
// when attempting to acquire this mutex you build?
//

// Based on The Little Book of Semaphores (Chapter 4.3)
// https://greenteapress.com/wp/semaphores/

typedef struct __ns_mutex_t {
    int room1;
    int room2;
    sem_t mutex;
    sem_t turnstile1;
    sem_t turnstile2;
} ns_mutex_t;

void ns_mutex_init(ns_mutex_t *m) {
    m->room1 = 0;
    m->room2 = 0;
    Sem_init(&m->mutex, 1);
    Sem_init(&m->turnstile1, 1);
    Sem_init(&m->turnstile2, 0);
}

void ns_mutex_acquire(ns_mutex_t *m) {
    Sem_wait(&m->mutex);
    m->room1++;
    Sem_post(&m->mutex);

    Sem_wait(&m->turnstile1);
    Sem_wait(&m->mutex);
    m->room1--;
    m->room2++;
    if (m->room1 == 0) {
        Sem_post(&m->mutex);
        Sem_post(&m->turnstile2);
    } else {
        Sem_post(&m->mutex);
        Sem_post(&m->turnstile1);
    }

    Sem_wait(&m->turnstile2);
    m->room2--;
}

void ns_mutex_release(ns_mutex_t *m) {
    if (m->room2 == 0) {
        Sem_post(&m->turnstile1);
    } else {
        Sem_post(&m->turnstile2);
    }
}

ns_mutex_t mutex;

// Per-thread counters
// If the no-starving-mutex works correctly,
// values of counters should be similar at the end of execution
int *counters = 0;

// total times the mutex will be acquired across all threads
int max_iterations = 0;
int iterations = 0;

void *worker(void *arg) {
    size_t id = (size_t) arg;
    int done = 0;
    while (!done) {
        ns_mutex_acquire(&mutex);
        if (iterations < max_iterations) {
            counters[id]++;
            iterations++;
        } else {
            done = 1;
        }
        ns_mutex_release(&mutex);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    assert(argc == 3);
    int num_threads = atoi(argv[1]);
    max_iterations = atoi(argv[2]);
    assert(num_threads > 0);
    assert(max_iterations > 0);

    counters = calloc(num_threads, sizeof(counters[0]));
    assert(counters);

    pthread_t threads[num_threads];
    ns_mutex_init(&mutex);

    printf("parent: begin\n");
    for (int i = 0; i < num_threads; i++) {
        Pthread_create(&threads[i], 0, worker, (void *)((size_t) i));
    }
    for (int i = 0; i < num_threads; i++) {
        Pthread_join(threads[i], 0);
    }    
    printf("parent: end\n");

    for (int i = 0; i < num_threads; i++) {
        printf("counters[%d]\t%d\n", i, counters[i]);
    }    

    return 0;
}

