#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int odd_nums[16] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31};

void *routine(void *arg)
{
    size_t offset = *(size_t *)arg;
    int sum = 0;
    for (size_t i = 0; i < 4; i++) {
        sum += odd_nums[offset + i];
    }
    printf("local sum: %d\n", sum);
    *(int *)arg = sum;
    return arg;
}

int main()
{
    pthread_t th[4];

    for (int i = 0; i < 4; i++) {
        int *p = malloc(sizeof(int));
        *p = i * 4;
        if (pthread_create(&th[i], NULL, &routine, p) != 0) {
            perror("Failed to create thread!\n");
        }
    }
    int global_sum = 0;
    for (int i = 0; i < 4; i++) {
        int *rtr = NULL;
        if (pthread_join(th[i], (void **)&rtr) != 0) {
            perror("Failed to join thread!\n");
        }
        global_sum += *rtr;
        free(rtr);
    }
    printf("global sum: %d\n", global_sum);
    return 0;
}
