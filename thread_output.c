#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void *roll_dice()
{
    int dice_value = (rand() % 6) + 1;
    int* res = malloc(sizeof(int));
    *res = dice_value;
    printf("thread res: %p\n", (void*)res);
    /* printf("res value: %d\n", *res); */
    return res;
}

int main()
{
    srand((unsigned int)time(NULL));
    pthread_t thr;
    if (pthread_create(&thr, NULL, roll_dice, NULL) != 0) {
        return 1;
    }
    int *res;
    if (pthread_join(thr, (void**)&res) != 0) {
        return 1;
    }
    printf("man res: %p\n", (void*)res);
    printf("Result: %d\n", *res);

    free(res);

    return 0;
}
