

#include <stdio.h>
#include <unistd.h>

#include <pthread.h>

pthread_barrier_t barrier;

#define THREAD_NUM 6

void* routine(void* args) {
    printf("Waiting at the barrier...\n");
    pthread_barrier_wait(&barrier);
    printf("Passed the barrier!\n");
    return NULL;
}

int main() {
    pthread_t th[THREAD_NUM];

    pthread_barrier_init(&barrier, NULL, 3);

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
    pthread_barrier_destroy(&barrier);
    return 0;
}
