

#include <stdio.h>
#include <unistd.h>

#include <pthread.h>
#include <semaphore.h>

#define THREAD_NUM 4

sem_t sem;

void* routine(void* args) {
    sem_wait(&sem); // if sem > 0 then (sem-- and continue) else wait().

    sleep(1);
    const int id = *(int*)args;
    printf("Hello from thread %d\n", id);

    sem_post(&sem); // sem++

    return NULL;
}

int main() {
    pthread_t th[THREAD_NUM];
    sem_init(&sem, 0, 2); // num_processes, initial_value (try values like 1, 2, 3)

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
