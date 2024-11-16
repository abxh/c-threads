#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define N 8

int fuel = 0;
pthread_mutex_t fuelMutex;
int water = 0;
pthread_mutex_t waterMutex;

void* routine() {

    // Potential deadlock can be simulated like so (in a very large program lets say)
    if (rand() % 2 == 0) {
        pthread_mutex_lock(&fuelMutex);
        sleep(1);
        pthread_mutex_lock(&waterMutex);
    } else {
        pthread_mutex_lock(&waterMutex);
        pthread_mutex_lock(&fuelMutex);
    }

    /*
    pthread_mutex_lock(&fuelMutex);
    // pthread_mutex_lock(&fuelMutex); // -> Deadlock
    pthread_mutex_lock(&waterMutex);
    */

    fuel += 50;
    water = fuel;

    printf("Incremented fuel to %d and set water to %d\n", fuel, water);
    pthread_mutex_unlock(&fuelMutex);
    pthread_mutex_unlock(&waterMutex);
    return NULL;
}


int main()
{
    pthread_t t[N];
    pthread_mutex_init(&fuelMutex, NULL);
    pthread_mutex_init(&waterMutex, NULL);

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
    pthread_mutex_destroy(&waterMutex);
    return 0;
}
