#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int pid = fork();
    if (pid == -1) {
        return 1;
    }
    int x = 2;

    const bool in_root_process = pid == 0;
    if (in_root_process) {
        x++;   
    }
    sleep(2);

    /* printf("Hello from processes!\n"); */
    printf("Value of x: %d\n", x);
    printf("Process id: %d\n", getpid());


    if (pid != 0) {
        wait(NULL);
    }
    return 0;
}
