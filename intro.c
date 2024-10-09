#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *routine()
{
    printf("Opening thread!\n");
    sleep(3);
    printf("Closing thread.\n");
    return NULL;
}

// pthread_create( thread, attrs, func, parameters)
// pthread_join(thread, func_w_rtr_value)

int main()
{
    pthread_t t1;
    pthread_t t2;
    if (pthread_create(&t1, NULL, routine, NULL)) {
        return 1;
    }
    if (pthread_create(&t2, NULL, routine, NULL)) {
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
