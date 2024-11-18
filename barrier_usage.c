

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>

#define THREAD_NUM 8

pthread_barrier_t barrier1;
pthread_barrier_t barrier2;
int diceValues[THREAD_NUM];
int status[THREAD_NUM] = {0};

void *routine(void *args)
{
    const int index = *(int *)args;

    while (true) {
        pthread_barrier_wait(&barrier1);

        diceValues[index] = (rand() % 6) + 1;

        pthread_barrier_wait(&barrier2);

        if (status[index] == 1) {
            printf("(%d rolled %d) I won\n", index, diceValues[index]);
        }
        else {
            printf("(%d rolled %d) I lost\n", index, diceValues[index]);
        }
    }

    return NULL;
}

int main()
{
    srand(time(NULL));
    pthread_t th[THREAD_NUM];

    pthread_barrier_init(&barrier1, NULL, THREAD_NUM + 1);
    pthread_barrier_init(&barrier2, NULL, THREAD_NUM + 1);

    int id[THREAD_NUM];
    for (int i = 0; i < THREAD_NUM; i++) {
        id[i] = i;
        if (pthread_create(&th[i], NULL, &routine, &id[i]) != 0) {
            perror("Failed to create thread");
        }
    }

    while (true) {
        pthread_barrier_wait(&barrier1);

        int max = 0;
        for (int i = 0; i < THREAD_NUM; i++) {
            if (diceValues[i] > max) {
                max = diceValues[i];
            }
        }
        for (int i = 0; i < THREAD_NUM; i++) {
            if (diceValues[i] == max) {
                status[i] = 1;
            }
            else {
                status[i] = 0;
            }
        }
        pthread_barrier_wait(&barrier2);
        sleep(1);
        printf("=== NEW ROUND STARTED ===\n");
    }
    for (int i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }

    pthread_barrier_destroy(&barrier1);
    pthread_barrier_destroy(&barrier2);

    return 0;
}
