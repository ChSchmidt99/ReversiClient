#ifndef PROCESS_H
#define PROCESS_H

#include "shareddataaccess/boarddataaccess.h"
#include "shareddataaccess/gamedataaccess.h"
#include "communicator/connection.h"
#include "misc/config.h"
#include <sys/types.h>

#define MAX_OPPONENTS 4

typedef struct _ProcessInfo ProcessInfo;

typedef struct _InitialSharedData {
    size_t boardSize;
    char* gameName;
    int moveTime;
    PlayerMeta* ownInfo;
    size_t opponentCount;
    PlayerMeta* opponents[MAX_OPPONENTS];
} InitialSharedData;

typedef struct _ProcessManagementInput {
    int(*preForkHandler)(Connection* connection, InitialSharedData *initSharedDataOut);
    int(*thinkerEntry)(ProcessInfo* processInfo);
    int(*communicatorEntry)(ProcessInfo* processInfo);
    Params* params;
} ProcessManagementInput;

int startProcessManagement(ProcessManagementInput* handlers);
int readFileDescriptor(ProcessInfo* info);
int writeFileDescriptor(ProcessInfo* info);

#endif
