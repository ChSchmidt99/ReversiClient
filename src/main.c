#include "optionreader.h"
#include "config.h"
#include "communicator/connection.h"
#include "communicator/prolog.h"
#include "shareddataaccess/gamedataaccess.h"
#include "thinker/thinker.h"
#include "utilities.h"
#include "communicator/gamesequence.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


#define VERSION_NUMBER "2.3"
#define DEFAULT_CONFIG_PATH "./client.conf"

int parentProcess(int argc, char *argv[],GameDataSHM* sharedMem);
int childProcess(int argc, char *argv[],GameDataSHM* sharedMem);

int main(int argc, char *argv[]) {
    pid_t processID;
    
    GameDataSHM* sharedMem = createGameDataSHM();

    /*
    int fd[2];
    if(pipe(fd) < 0) {
        panic("Failed to create pipe");
    }
    */

    if((processID = fork()) < 0) {
        panic("Failed to fork");
    } else if (processID == 0){
        return parentProcess(argc,argv,sharedMem);
    } else {
        return childProcess(argc,argv,sharedMem);
    }

    clearGameDataSHM(sharedMem);
}

int parentProcess(int argc, char *argv[], GameDataSHM* sharedMem){
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

int childProcess(int argc, char *argv[], GameDataSHM* sharedMem){
    return EXIT_SUCCESS;
    /*
    printf("[PARENT/%i] Started connector child\n", thinker);

    // start the thinker process
    printf("[PARENT] Starting thinker..\n");
    tick(NULL, fd);

    if((waitpid (thinker, NULL, 0)) < 0) {
        perror ("Error waiting for child processes to die");
        exit (EXIT_FAILURE);
    }

    printf("Connector died, exiting..\n");
    */
}