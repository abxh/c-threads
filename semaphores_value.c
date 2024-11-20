#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 8

sem_t sem;

void *routine()
{
    sem_wait(&sem);

    int semVal;
    sem_getvalue(&sem, &semVal);

    printf("%lu has semaphore value: %d\n", pthread_self(), semVal);

    sem_post(&sem);
    return NULL;
}

int main()
{
    pthread_t t[N];
    sem_init(&sem, 0, 3);

    for (size_t i = 0; i < N; i++) {
        if (pthread_create(&t[i], NULL, routine, NULL) != 0) {
            perror("failed to join thread!");
        }
    }

    for (size_t i = 0; i < N; i++) {
        if (pthread_join(t[i], NULL) != 0) {
            perror("failed to join thread!");
        }
    }

    sem_destroy(&sem);

    return 0;
}
