#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "thinker/thinker.h"
#include "thinker/field.h"

void tick(char* shm, int pipe[]) {
    close(pipe[0]);
    sigset_t mask, oldMask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &mask, &oldMask);

    printf("Thinker is now waiting for signals (SIGUSR1)\n");
    int f = 0;
    while(f++ < 100) {
        sigsuspend(&oldMask);
        sigprocmask(SIG_UNBLOCK, &mask, NULL);

        printf("Got signal from communicator, waking up..");
        printf("Loading current board");
        printf("%s", shm);
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

        char* move = toString(x, y);
        write(pipe[1], move, sizeof(move) + 1);
        free(move);*/
        printf("Sent move to connector, sleeping again");
    }
}