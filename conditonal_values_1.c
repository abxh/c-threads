#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

pthread_mutex_t stoveMutex[4];
int stove[4] = {100, 100, 100, 100};

int DID_NOT_COOK = 0;
int COOKED = 1;

void *routine()
{
    while (true) {
        for (size_t i = 0; i < 4; i++) {
            if (pthread_mutex_trylock(&stoveMutex[i]) == 0) {
                printf("Cooking with stove %zu\n", i);
                int fuelNeeded = (rand() % 30);
                if (stove[i] - fuelNeeded < 0) {
                    printf("No more fuel.. going home\n");
                    pthread_mutex_unlock(&stoveMutex[i]);
                    return &DID_NOT_COOK;
                }
                else {
                    stove[i] -= fuelNeeded;
                    usleep(500 * (int)1e+3);
                    printf("Stove %zu with new fuel: %d\n", i, stove[i]);
                }
                pthread_mutex_unlock(&stoveMutex[i]);
                return &COOKED;
            }
        }
        printf("No stove available yet. Waiting...\n");
        usleep(500 * (int)1e+3);
    }
}

#define N 32

int main()
{
    srand((unsigned int)time(NULL));

    for (size_t i = 0; i < 4; i++) {
        pthread_mutex_init(&stoveMutex[i], NULL);
    }

    pthread_t t[N];
    for (size_t i = 0; i < N; i++) {
        if (pthread_create(&t[i], NULL, routine, NULL) != 0) {
            perror("Failed to create thread!\n");
            return 1;
        }
    }
    int total = 0;
    for (size_t i = 0; i < N; i++) {
        int *ret;
        if (pthread_join(t[i], (void*)&ret) != 0) {
            perror("Failed to join thread!\n");
            return 1;
        }
        total += *ret;
    }
    for (size_t i = 0; i < N; i++) {
        pthread_mutex_destroy(&stoveMutex[i]);
    }
    printf("------------------\n");
    printf("Chefs that cooked: %d\n", total);
    printf("Total number of chefs: %d\n", N);

    return 0;
}
