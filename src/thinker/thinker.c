#include "thinker_priv.h"

#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#include "thinker/field.h"
#include "utilities.h"

//TODO: Move to core and centralize with communicator
#define MOVE_BUFFER_SIZE 3

//TODO: Is there a better way than using global vars=
int isRunning;
BoardSHM* boardSharedMem; 
GameDataSHM* gameSharedMem; 
ProcessInfo* processInfo;
int errorFlag = 0;

int startThinker(BoardSHM* boardSHM, GameDataSHM* gameSHM, ProcessInfo* procInfo){
    boardSharedMem = boardSHM;
    gameSharedMem = gameSHM;
    processInfo = procInfo;
    
    if (addSignalHandler() == -1){
        perror("Failed to add Signal handler");
        return -1;
    }
    return 0;
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
    //TODO: Do thinking
    /*
    printf("Board in shared Mem: \n");
    size_t boardSize;
    char (*board)[boardSize] = getBoard(boardSharedMem, &boardSize);
    printf("BoardSize: %zu\n",boardSize);
    
    for (size_t i = 0; i < boardSize; i++){
        for (size_t j = 0; j < boardSize; j++){
            printf("%c ",board[i][j]);
        }
        printf("\n");
    }
    */

    printf("Thinker Received Think Signal\n");

    //TODO: Check if isThinking is checked
    //TODO: Replace by real move
    char buff[MOVE_BUFFER_SIZE] = {'E','3','\0'};
    PlayerMeta* playerInfo = getOwnPlayerMeta(gameSharedMem);
    if (playerInfo->number == 1){
        buff[1] = '5'; 
    }

    printf("Writing move %s to Pipe with fd: %i ...\n",buff,writeFileDescriptor(processInfo));
    ssize_t writtenSize = write(writeFileDescriptor(processInfo),buff,MOVE_BUFFER_SIZE);
    
    if (writtenSize != MOVE_BUFFER_SIZE){
        perror("Failed to write to Pipe!");
    } else {
        printf("Successfuly wrode to Pipe!\n");
    }

    setIsThinking(gameSharedMem, 0);
}