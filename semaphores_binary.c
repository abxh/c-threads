#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define N 8

int* fuel;
sem_t sem;


void* routine() {
    *fuel += 50;
    printf("Incremented fuel to %d\n", *fuel);
    sem_post(&sem);
    return NULL;
}


int main()
{
    pthread_t t[N];
    sem_init(&sem, 0, 0);

    fuel = malloc(sizeof(int));
    *fuel = 0;

    for (size_t i = 0; i < N; i++) {
        if (pthread_create(&t[i], NULL, routine, NULL) != 0) {
            perror("failed to join thread!");
        }
    }
    for (size_t i = 0; i < N; i++) {
        sem_wait(&sem);
    }
    printf("Deallocating fuel\n");
    free(fuel);

    for (size_t i = 0; i < N; i++) {
        if (pthread_join(t[i], NULL) != 0) {
            perror("failed to join thread!");
        }
    }
    
    sem_destroy(&sem);

    return 0;
}
