#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "thinker/thinker.h"
#include "thinker/field.h"

void tick(char* shm, int pipe[]) {
    close(pipe[0]);

    sigset_t sigset;
    int sig;
    sigemptyset(&sigset);
    if(sigaddset(&sigset, SIGUSR1) == -1) {
        perror("sigaddset");
    }
    sigprocmask(SIG_BLOCK, &sigset, NULL);
    printf("Thinker is now waiting for signals (SIGUSR1)\n");
    if(sigwait(&sigset, &sig) != 0) {
        perror("sigwait");
    }
    printf("Got signal %i, exiting..\n", sig);
    if(shm != NULL)
        printf("%s", shm);
}