

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>
#include <semaphore.h>

#define THREAD_NUM 16

sem_t sem;

void* routine(void* args) {
    const int id = *(int*)args;

    printf("%d: Waiting in the login queue\n", id);
    sem_wait(&sem); // if sem > 0 then (sem-- and continue) else wait().

    printf("(%d) Logged in\n", id);
    sleep(rand() % 5 + 1);

    printf("(%d) Logged out\n", id);

    sem_post(&sem); // sem++

    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t th[THREAD_NUM];
    sem_init(&sem, 0, 12); // num_processes, initial_value (try values like 1, 2, 3)

    int id[THREAD_NUM];
    for (int i = 0; i < THREAD_NUM; i++) {
        id[i] = i;
        if (pthread_create(&th[i], NULL, &routine, &id[i]) != 0) {
            perror("Failed to create thread");
        }
    }
    for (int i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    sem_destroy(&sem);
    return 0;
}
