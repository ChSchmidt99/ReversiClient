#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>
#include "shareddataaccess/boarddataaccess.h"
#include "shareddataaccess/gamedataaccess.h"
#include "communicator/connection.h"
#include "misc/config.h"
#include "core.h"


typedef struct _InitialSharedData {
    size_t boardSize;
    char* gameName;
    int moveTime;
    PlayerMeta* ownInfo;
    size_t opponentCount;
    PlayerMeta* opponents[MAX_OPPONENTS];
} InitialSharedData;

typedef struct _ProcessManagementInput {
    int(*preForkHandler)(Connection* connection, InputParams* inputParams,InitialSharedData *initSharedDataOut);
    int(*thinkerEntry)(BoardSHM* boardSHM, GameDataSHM* gameSHM);
    int(*communicatorEntry)(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM);
    InputParams* inputParams;
} ProcessManagementInput;

int startProcessManagement(ProcessManagementInput* handlers);

int readFileDescriptor(int fd[2]);
int writeFileDescriptor(int fd[2]);
#endif
