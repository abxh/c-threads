#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void *roll_dice(void *arg)
{
    int dice_value = (rand() % 6) + 1;
    *(int *)arg = dice_value;
    printf("thread res: %p\n", arg);
    printf("thread res value: %d\n", dice_value);
    return arg;
}

int main()
{
    srand((unsigned int)time(NULL));
    pthread_t thr;
    {
        int *inp = malloc(sizeof(int));
        if (pthread_create(&thr, NULL, roll_dice, inp) != 0) {
            return 1;
        }
    }
    {
        int *out = NULL;
        if (pthread_join(thr, (void **)&out) != 0) {
            return 1;
        }
        printf("main() res: %p\n", (void *)out);
        printf("main() res value: %d\n", *out);
        free(out);
    }

    return 0;
}
