#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int mails = 0;

void* routine() {
    for (size_t i = 0; i < 1000000; i++) {
        mails++;
        // behind the scenes:
        // - load mails to some register r0
        // - increment r0
        // - write r0 to mails
    }
    return NULL;
}

/*
    Race condition:

        Best case:
        t1 [read mails, increment, write mails]
                                               ->
                                                 t2 [read mails, increment, write mails]
                        t1      t2
        read mails:     23      24
        increment:      23      24
        write mails:    24      25

        Bad case:
        t1 [read mails,              increment, write mails] -> ...
                   ->              ->                              ->
                     t2 [read mails,                                 increment, write mails]
                        t1      t2
        read mails:     29      23
        increment:      29      24
        write mails:    30      24

*/

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
    printf("Number of mails: %d\n", mails);
    return 0;
}
