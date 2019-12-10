#include "optionreader.h"
#include "config.h"
#include "communicator/connection.h"
#include "communicator/prolog.h"
#include "shareddataaccess/shareddataaccess.h"
#include "thinker/thinker.h"
#include "utilities.h"
#include "communicator/gamesequence.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include "info/process.h"

#define VERSION_NUMBER "2.3"
#define DEFAULT_CONFIG_PATH "./client.conf"

int parentProcess(int argc, char *argv[],SharedMemory* sharedMem, ProcessInfo* procInfo);
int childProcess(int argc, char *argv[],SharedMemory* sharedMem, ProcessInfo* procInfo);

int main(int argc, char *argv[]) {
    pid_t processID;
    pid_t parentId = getpid();
    
    SharedMemory* sharedMem = createSharedMemory();
    ProcessInfo* processInfo = createProcessInfo();
    setProcParent(processInfo, &parentId);

    if((processID = fork()) < 0) {
        panic("Failed to fork");
    } else if (processID > 0){
        setProcChild(processInfo, &processID);

        return parentProcess(argc,argv,sharedMem, processInfo);
    } else {
        return childProcess(argc,argv,sharedMem, processInfo);
    }

    clearSharedData(sharedMem);
}

int childProcess(int argc, char *argv[], SharedMemory* sharedMem, ProcessInfo* procInfo){
    char* gameId = readGameID(argc,argv);
    if (gameId == NULL) {
        printf("GameId must be set!\n");
        return EXIT_FAILURE;
    }
    char* playerPreference = readPreferencedPlayerNumber(argc,argv);

    char* configFilePath = readConfigFilePath(argc, argv);
    if (configFilePath == NULL) {
        configFilePath = DEFAULT_CONFIG_PATH;
    }

    Params* params = getParamsFromFile(configFilePath);

    Connection* connection = newConnection(params->hostName,params->portNumber);
    freeParams(params);
    connectToServer(connection);

    GameInstance* gameInstance = initiateProlog(connection,VERSION_NUMBER,gameId, playerPreference);
    printGameInstanceDetails(gameInstance);
    
    startGameLoop(connection, sharedMem);

    freeGameInstance(gameInstance);

    disconnectFromServer(connection);
    freeConnection(connection);
    free(gameId);
    free(playerPreference);

    printf("[CONNECTOR] Finished prolog, exiting..\n");
    //kill(thinker, SIGKILL);
    return EXIT_SUCCESS;
}

int parentProcess(int argc, char *argv[], SharedMemory* sharedMem, ProcessInfo* procInfo){
    // start the thinker process
    tick(sharedMem, procInfo);

    if((waitpid (getProcChild(procInfo), NULL, 0)) < 0) {
        perror ("Error waiting for child processes to die");
        exit (EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}