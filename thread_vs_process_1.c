#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

int x = 2;

void* routine() {
    /* printf("Hello from threads!\n"); */
    x++;
    printf("thread id: %d\n", getpid());
    sleep(2);
    printf("%d\n", x);
    return NULL;
}

void* routine2() {
    printf("thread id: %d\n", getpid());
    sleep(2);
    printf("%d\n", x);
    return NULL;
}


int main() {
    pthread_t t1, t2;
    if (pthread_create(&t1, NULL, &routine, NULL)) {
        return 1;
    }
    if (pthread_create(&t2, NULL, &routine2, NULL)) {
        return 1;
    }
    if (pthread_join(t1, NULL)) {
        return 1;
    }
    if (pthread_join(t2, NULL)) {
        return 1;
    }
    return 0;
}
