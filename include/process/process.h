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

typedef struct _InputParams {
    ConfigParams* configParams;
    char* gameId;
    char* playerPreference;
} InputParams;

typedef struct _ProcessManagementInput {
    int(*preForkHandler)(Connection* connection, InputParams* inputParams,InitialSharedData *initSharedDataOut);
    int(*thinkerEntry)(ProcessInfo* processInfo, BoardSHM* boardSHM, GameDataSHM* gameSHM);
    int(*communicatorEntry)(ProcessInfo* processInfo, Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM);
    InputParams* inputParams;
} ProcessManagementInput;

int startProcessManagement(ProcessManagementInput* handlers);

int readFileDescriptor(ProcessInfo* info);
int writeFileDescriptor(ProcessInfo* info);

pid_t getChildPID(ProcessInfo* info);
pid_t getParentPID(ProcessInfo* info);
#endif
