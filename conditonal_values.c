#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

pthread_mutex_t mutexFuel;
pthread_cond_t condFuelMayHaveChanged;
int fuel = 0;

void *fuel_up()
{
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&mutexFuel);
        fuel += 15;
        printf("Filling fuel~, %d\n", fuel);
        pthread_mutex_unlock(&mutexFuel);
        pthread_cond_signal(&condFuelMayHaveChanged);
        sleep(1);
    }
    return NULL;
}

void *car()
{
    printf("Here to get fuel!\n");

    /* Possible hacky alternative:
    while (true) {
        pthread_mutex_lock(&mutex);
        if (fuel >= 40) {
            break;
        }
        printf("Fuel is too low. Waiting..\n");
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    */

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

int main(void)
{
    pthread_t t[2];
    pthread_mutex_init(&mutexFuel, NULL);
    pthread_cond_init(&condFuelMayHaveChanged, NULL);
    if (pthread_create(&t[0], NULL, car, NULL) != 0) {
        perror("thread with car() couldn't be started!\n");
        return 1;
    }
    if (pthread_create(&t[1], NULL, fuel_up, NULL) != 0) {
        perror("thread with fuel_up() couldn't be started!\n");
        return 1;
    }
    for (size_t i = 0; i < 2; i++) {
        if (pthread_join(t[i], NULL) != 0) {
            perror("failed to join() thread\n");
        }
    }
    pthread_mutex_destroy(&mutexFuel);
    pthread_cond_destroy(&condFuelMayHaveChanged);
    return 0;
}
