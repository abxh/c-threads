#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

void* routine(void* arg) {
    sleep(1);
    int index = *(int*)arg;
    /* printf("%d ", index); */
    printf("%d ", primes[index]);
    return NULL;
}

int main() {
    pthread_t th[10];
    /* size_t i; */

    int* val_ptrs[10];
    for (int i = 0; i < 10; i++) {
        val_ptrs[i] = malloc(sizeof(int));
        *val_ptrs[i] = i;
        if (pthread_create(&th[i], NULL, &routine, val_ptrs[i]) != 0) {
            perror("Failed to create thread!\n");
        }
    }
    for (int i = 0; i < 10; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread!\n");
        }
        free(val_ptrs[i]);
    }
    return 0;
}
