#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define N 8

int fuel = 0;
pthread_mutex_t fuelMutex;


void* routine() {
    pthread_mutex_lock(&fuelMutex);
    pthread_mutex_lock(&fuelMutex);
    fuel += 50;
    printf("Incremented fuel to %d\n", fuel);
    pthread_mutex_unlock(&fuelMutex);
    pthread_mutex_unlock(&fuelMutex);
    return NULL;
}


int main()
{
    pthread_t t[N];

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

    pthread_mutex_init(&fuelMutex, &attr);

    pthread_mutexattr_destroy(&attr);

    for (size_t i = 0; i < N; i++) {
        if (pthread_create(&t[i], NULL, routine, NULL)) {
            perror("failed to join thread!");
        }
    }
    for (size_t i = 0; i < N; i++) {
        if (pthread_join(t[i], NULL)) {
            perror("failed to join thread!");
        }
    }
    pthread_mutex_destroy(&fuelMutex);

    return 0;
}
