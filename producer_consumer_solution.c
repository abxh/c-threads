/*
    1 to n producers
    1 to m consumers

    Problems:
        - Manage shared memory access
        - Checking for if buffer is full
        - Checking for if buffer is empty
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdbool.h>
#include <unistd.h>

#define THREAD_NUM 7

int buffer[10];
int count;

pthread_mutex_t bufferMutex;
sem_t semEmpty;
sem_t semFull;

void* producer(void* args) {
    while (true) {
        sem_wait(&semEmpty);

        pthread_mutex_lock(&bufferMutex);
        buffer[count++] = rand() % 100;
        pthread_mutex_unlock(&bufferMutex);

        sem_post(&semFull);

        sleep(1);
    }

    return NULL;
}

void* consumer(void* args) {

    while (true) {
        sem_wait(&semFull);

        pthread_mutex_lock(&bufferMutex);
        printf("Got %d\n", buffer[--count]);
        pthread_mutex_unlock(&bufferMutex);

        sem_post(&semEmpty);
    }

    return NULL;
}

int main(void) {
    srand(time(NULL));
    pthread_t th[THREAD_NUM];
    sem_init(&semEmpty, 0, 10);
    sem_init(&semFull, 0, 0);
    for (int i = 0; i < THREAD_NUM; i++) {
        if (i % 2 == 0) {
            if (pthread_create(&th[i], NULL, &producer, NULL) != 0) {
                perror("failed to create thread");
            }
        } else {
            if (pthread_create(&th[i], NULL, &consumer, NULL) != 0) {
                perror("failed to create thread");
            }
        }
    }
    for (int i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("failed to join thread");
        }
    }
    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
}
