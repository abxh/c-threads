#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int mails = 0;
pthread_mutex_t mutex; // sort of like: bool should_lock;

void* routine() {
    for (size_t i = 0; i < 1000000; i++) {
        pthread_mutex_lock(&mutex); // if (should_lock) { wait() }; should_lock = true;
        mails++;
        /*
            mails++:
                - load mails to register r0
                - increment r0
                - write r0 to mails
        */
        // mutex locks all threads trying to run this for loop chunk
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

#define N 8

int main()
{
    pthread_t t[N];
    pthread_mutex_init(&mutex, NULL); // should_lock = false

    for (size_t i = 0; i < N; i++) {
        if (pthread_create(&t[i], NULL, routine, NULL)) {
            fprintf(stderr, "failed to create thread!\n");
            return 1;
        }  else {
            printf("thread %zu created!\n", i);
        }  
    }
    // The for loops split is *very* important to not running only one thread at a single time
    for (size_t i = 0; i < N; i++) {
        if (pthread_join(t[i], NULL)) { // wait for the thread to finish
            return 1;
        } else {
            printf("thread %zu finished!\n", i);
            // doesn't neccesary mean 0 -> 1 -> 2 is the order in which threads finish
            // just means, 2 -> 0 (join(thread_0))
            //             2 -> 0 -> 1 (join(thread_1))
            //             2 -> 0 -> 1 (join(thread_2))
            // 2 may finish before 0. but join(thread_0) may stop freezing only after 2 and 0 are finished
        }
    }
    pthread_mutex_destroy(&mutex);
    printf("Number of mails: %d\n", mails);
    return 0;
}
