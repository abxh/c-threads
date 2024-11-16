#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define THREAD_NUM 2

void* routine() {
    sleep(1);
    printf("Thread has run!\n");
    return NULL;
}

int main(void) {
    pthread_t th[THREAD_NUM];

    // better:
    pthread_attr_t threadAttr;
    pthread_attr_init(&threadAttr);
    pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);

    for (int i = 0; i < THREAD_NUM; i++) {
        if (pthread_create(&th[i], &threadAttr, &routine, NULL) != 0) {
            perror("Failed to create thread");
        }
        /* pthread_detach(th[i]); */
    }

    pthread_attr_destroy(&threadAttr);

    printf("Main thread is exiting!\n");
    pthread_exit(0);
    printf("Main thread is alive!");
}
