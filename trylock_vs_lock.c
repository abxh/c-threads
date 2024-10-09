#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutex;

/*
void *routine()
{
    pthread_mutex_lock(&mutex);
    printf("Got lock\n");
    sleep(1); // critical section
    pthread_mutex_unlock(&mutex);
    return NULL;
}
*/

void *routine()
{
    if (pthread_mutex_trylock(&mutex) == 0) {
        printf("Got lock\n");
        sleep(1);
        pthread_mutex_unlock(&mutex);
        return NULL;
    } else {
        printf("Did not get lock\n");
        return NULL;
    }
}

int main()
{
    pthread_t th[4];
    pthread_mutex_init(&mutex, NULL);
    for (size_t i = 0; i < 4; i++) {
        if (pthread_create(&th[i], NULL, routine, NULL) != 0) {
            return 1;
        }
    }
    for (size_t i = 0; i < 4; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            return 1;
        }
    }
    return 0;
}
