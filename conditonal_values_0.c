#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutexFuel;
pthread_cond_t condFuelMayHaveChanged;
int fuel = 0;

void *fuel_up()
{
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&mutexFuel);
        fuel += 30; // prev: 15
        printf("Filling fuel~, %d\n", fuel);
        pthread_mutex_unlock(&mutexFuel);
        /* pthread_cond_signal(&condFuelMayHaveChanged); */ // to only one thread at random
        pthread_cond_broadcast(&condFuelMayHaveChanged);
        sleep(1);
    }
    return NULL;
}

void *car()
{
    while (fuel < 40) {
        printf("Fuel is too low. Waiting..\n");
        pthread_cond_wait(&condFuelMayHaveChanged, &mutexFuel);
        // equivalent to:
        // - unlock(&mutexFuel)
        // - wait() for condFuelMayHaveChanged signal
        // - lock(&mutexFuel)
    }

    fuel -= 40;
    printf("Got fuel. Now left: %d\n", fuel);
    pthread_mutex_unlock(&mutexFuel);

    return NULL;
}

#define N 6

int main(void)
{
    pthread_t t[N];
    pthread_mutex_init(&mutexFuel, NULL);
    pthread_cond_init(&condFuelMayHaveChanged, NULL);

    for (size_t i = 0; i < N; i++) {
        if (i == 4 || i == 5) {
            if (pthread_create(&t[i], NULL, fuel_up, NULL) != 0) {
                perror("thread with fuel_up() couldn't be started!\n");
                return 1;
            }
        }
        else {
            if (pthread_create(&t[i], NULL, car, NULL) != 0) {
                perror("thread with car() couldn't be started!\n");
                return 1;
            }
        }
    }

    for (size_t i = 0; i < N; i++) {
        if (pthread_join(t[i], NULL) != 0) {
            perror("failed to join() thread\n");
        }
    }
    pthread_mutex_destroy(&mutexFuel);
    pthread_cond_destroy(&condFuelMayHaveChanged);
    return 0;
}
