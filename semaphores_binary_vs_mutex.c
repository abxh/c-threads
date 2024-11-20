#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 8

int fuel = 0;
sem_t sem;
pthread_mutex_t mutex;

void *routine1()
{
    while (true) {
        pthread_mutex_lock(&mutex);
        /* sem_wait(&sem); */
        fuel += 50;
        printf("fuel value: %d\n", fuel);
    }
    return NULL;
}

void *routine2()
{
    while (true) {
        pthread_mutex_unlock(&mutex); // UB!! mutex'es cannot be shared between threads and are owned by threads.
        /* sem_post(&sem); */
        usleep(5000);
    }
    return NULL;
}

int main()
{
    pthread_t t[N];
    sem_init(&sem, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    for (size_t i = 0; i < N; i++) {
        if (i % 2 == 0 && pthread_create(&t[i], NULL, routine1, NULL) != 0) {
            perror("failed to join thread!");
        }
        else if (i % 2 == 1 && pthread_create(&t[i], NULL, routine2, NULL) != 0) {
            perror("failed to join thread!");
        }
    }

    for (size_t i = 0; i < N; i++) {
        if (pthread_join(t[i], NULL)) {
            perror("failed to join thread!");
        }
    }

    sem_destroy(&sem);
    pthread_mutex_destroy(&mutex);

    return 0;
}
