#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "thinker/thinker.h"
#include "thinker/field.h"

void tick(SharedMemory* shm, ProcessInfo* procInfo) {
    close(readFileDescriptor(procInfo));

    sigset_t sigset;
    int sig;
    sigemptyset(&sigset);
    if(sigaddset(&sigset, SIGUSR1) == -1) {
        perror("sigaddset");
    }
    sigprocmask(SIG_BLOCK, &sigset, NULL);
    if(sigwait(&sigset, &sig) != 0) {
        perror("sigwait");
    }

    printf("Got signal from communicator, waking up..\n");
    printf("Loading current board\n");
    if(shm != NULL) {}
    /*
    Field* field = createField(8, 8);
    loadFieldFromSHM(field, shm);
    long int x = -1, y = -1;
    for(unsigned int j = 0; j < field->width; j++) {
        for(unsigned int i = 0 ; i < field->height; i++) {
            if(isValidMove(field, 'W', j, i)) {
                x = j;
                y = i;
                break;
            }
        }
    }

    printf("Found available move: %li %li", x, y);
    */

    /*
    char* move = toString(x, y);
    write(pipe[1], move, sizeof(move) + 1);
    free(move);
     */
    printf("Sent move to connector, sleeping again\n");
}