#include "thinker_priv.h"

#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#include "ai/mcst_api.h"
#include "misc/utilities.h"

//TODO: Move to core and centralize with communicator
#define MOVE_BUFFER_SIZE 3

typedef struct _Thinker {
    BoardSHM* boardSHM;
    GameDataSHM* gameSHM; 
} Thinker;

Thinker* thinker;

int initThinkerOnce(BoardSHM* boardSHM, GameDataSHM* gameSHM){
    thinker = safeMalloc(sizeof(Thinker));
    thinker->boardSHM = boardSHM;
    thinker->gameSHM = gameSHM;
    if (addSignalHandler() == -1){
        perror("Failed to add Signal handler");
        return -1;
    }
    return 0;
}

void deinitThinker(){
    free(thinker);
}

int addSignalHandler(){
    struct sigaction newSig;
    newSig.sa_handler = &handle_Signal;
    newSig.sa_flags = SA_RESTART;
    sigfillset(&newSig.sa_mask);
    return sigaction(SIGUSR1,&newSig,NULL);
}

void handle_Signal(int signal){
    switch (signal){
    case SIGUSR1:
        receivedThinkSignal();
        break;
    default:
        printf("Caught wrong signal");
        break;
    }
}

void receivedThinkSignal(){
    int isThinking = getIsThinking(thinker->gameSHM);
    if (!isThinking)
        return;

    printf("Thinker thinking...\n");
    size_t boardSize = getBoardSize(thinker->boardSHM);
    char (*board)[boardSize] = getBoard(thinker->boardSHM);
    PlayerMeta* playerInfo = getOwnPlayerMeta(thinker->gameSHM);
    
    char playerSymbol = 'B';
    if (playerInfo->number == 1)
        playerSymbol = 'W';

    int moveTime = getMoveTime(thinker->gameSHM);
    char* move = CalculateNextMoveAIOptimizedThreads(board,playerSymbol, moveTime - LATENCY_CORRECTION_TIME);

    ProcessInfo* processInfo = getProcessInfo(thinker->gameSHM);
    ssize_t writtenSize = write(writeFileDescriptor(processInfo->fd),move,MOVE_BUFFER_SIZE);
    freeProcessInfo(processInfo);

    if (writtenSize != MOVE_BUFFER_SIZE){
        perror("Failed to write to Pipe!");
    }

    free(board);
    freePlayerMeta(playerInfo);
    free(move);
    setIsThinking(thinker->gameSHM, 0);
    printf("Thinker Finished...\n");
}