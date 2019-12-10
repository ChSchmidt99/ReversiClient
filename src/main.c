#include "optionreader.h"
#include "config.h"
#include "communicator/connection.h"
#include "communicator/prolog.h"
#include "shareddataaccess/gamedataaccess.h"
#include "shareddataaccess/boarddataaccess.h"
#include "thinker/thinker.h"
#include "utilities.h"
#include "communicator/gamesequence.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


#define VERSION_NUMBER "2.3"
#define DEFAULT_CONFIG_PATH "./client.conf"

Connection* initiateConnectionSequence(int argc, char *argv[]);

int parentProcess(int argc, char *argv[], Connection* connection, BoardSHM* boardSHM);
int childProcess(int argc, char *argv[],BoardSHM* boardSHM);

int main(int argc, char *argv[]) {
    
    char* gameId = readGameID(argc,argv);
    if (gameId == NULL) {
        printf("GameId must be set!\n");
        return EXIT_FAILURE;
    }
    char* playerPreference = readPreferencedPlayerNumber(argc,argv);

    Connection* connection = initiateConnectionSequence(argc,argv);
    GameInstance* gameInstance = initiateProlog(connection,VERSION_NUMBER,gameId, playerPreference);
    printGameInstanceDetails(gameInstance);

    //GameDataSHM* sharedGameMem = createGameDataSHM();
    BoardSHM* boardSHM = createBoardSHM(gameInstance->boardSize);    

    freeGameInstance(gameInstance);

    pid_t processID;

    if((processID = fork()) < 0) {
        panic("Failed to fork");
    } else if (processID == 0){
        return parentProcess(argc,argv,connection, boardSHM);
    } else {
        return childProcess(argc,argv,boardSHM);
    }

    /*
    int fd[2];
    if(pipe(fd) < 0) {
        panic("Failed to create pipe");
    }
    */

    disconnectFromServer(connection);
    freeConnection(connection);
    free(gameId);
    free(playerPreference);
   
    if (detachBoardSHM(boardSHM) == -1)
        panic("Failed to detach boardSHM");
    if (clearBoardSHM(boardSHM) == -1)
        panic("Failed to clear boardSHM");
    //detachGameDataSHM(sharedGameMem);
    //clearGameDataSHM(sharedGameMem);
}

Connection* initiateConnectionSequence(int argc, char *argv[]){
    char* configFilePath = readConfigFilePath(argc, argv);
    if (configFilePath == NULL) {
        configFilePath = DEFAULT_CONFIG_PATH;
    }

    Params* params = getParamsFromFile(configFilePath);

    Connection* connection = newConnection(params->hostName,params->portNumber);
    freeParams(params);
    connectToServer(connection);
    return connection;
}

int parentProcess(int argc, char *argv[], Connection* connection, BoardSHM* boardSHM){
    
    startGameLoop(connection, boardSHM);

    printf("[CONNECTOR] Finished prolog, exiting..\n");
    //kill(thinker, SIGKILL);
    return EXIT_SUCCESS;
}

int childProcess(int argc, char *argv[], BoardSHM* boardSHM){
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