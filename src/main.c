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

#include <sys/wait.h>
#include "info/process.h"


#define VERSION_NUMBER "2.3"
#define DEFAULT_CONFIG_PATH "./client.conf"

Connection* initiateConnectionSequence(int argc, char *argv[]);
void teardownConnection(Connection* connection);
void teardownSHM(BoardSHM* boardSHM);

int parentProcess(BoardSHM* boardSHM, ProcessInfo* procInfo);
int childProcess(BoardSHM* boardSHM, Connection* connection, int moveTime, ProcessInfo* procInfo);

//TODO: Get strc+c signal and execute teardown!

int main(int argc, char *argv[]) {
    char* gameId = readGameID(argc,argv);
    if (gameId == NULL) {
        printf("GameId must be set!\n");
        return EXIT_FAILURE;
    }
    char* playerPreference = readPreferencedPlayerNumber(argc,argv);

    Connection* connection = initiateConnectionSequence(argc,argv);
    GameInstance* gameInstance = initiateProlog(connection,VERSION_NUMBER,gameId, playerPreference);
    if (gameInstance == (GameInstance*)-1){
        printf("Failed Prolog!");
        teardownConnection(connection);
        return EXIT_FAILURE;
    }
    
    printGameInstanceDetails(gameInstance);
    free(gameId);
    free(playerPreference);
    freeGameInstance(gameInstance);

    //TODO: Move to better spot!
    int moveTime = waitForFirstMove(connection);
    if (moveTime == -1){
        printf("Failed to wait for first Move!");
        teardownConnection(connection);
        return EXIT_FAILURE;
    }

    size_t rows = 0;
    size_t cols = 0;
    if (receiveBoardDimensions(connection,&rows,&cols) == -1){
        teardownConnection(connection);
        return EXIT_FAILURE;
    }
    
    //TODO: Use rows and cols instead of size
    BoardSHM* boardSHM = createBoardSHM(rows);



    pid_t parentId = getpid();
    
    ProcessInfo* processInfo = createProcessInfo();
    setProcParent(processInfo, &parentId);

    int exitCode = EXIT_SUCCESS;
    pid_t processID = fork();
    if (processID < 0){
        printf("Failed To Fork!\n");
        teardownSHM(boardSHM);
    } else if (processID == 0){
        //Child
        setProcChild(processInfo, &processID);
        exitCode = childProcess(boardSHM, connection, moveTime, processInfo);

        teardownConnection(connection);
        teardownSHM(boardSHM);
    } else {
        //Parent
        teardownConnection(connection);
        exitCode = parentProcess(boardSHM,processInfo);
        teardownSHM(boardSHM);
    }

    return exitCode;
}   

void teardownSHM(BoardSHM* boardSHM){
    if (detachBoardSHM(boardSHM) == -1)
        panic("Failed to detach boardSHM");
    if (clearBoardSHM(boardSHM) == -1)
        panic("Failed to clear boardSHM");
}

void teardownConnection(Connection* connection){
    disconnectFromServer(connection);
    freeConnection(connection);
}

Connection* initiateConnectionSequence(int argc, char *argv[]){
    char* configFilePath = readConfigFilePath(argc, argv);
    if (configFilePath == NULL) {
        configFilePath = DEFAULT_CONFIG_PATH;
    }

    Params* params = getParamsFromFile(configFilePath);

    Connection* connection = newConnection(params->hostName,params->portNumber);
    freeParams(params);
    if (connectToServer(connection) == -1)
        panic("Failed to connect to server");
    return connection;
}

int parentProcess(BoardSHM* boardSHM, ProcessInfo* procInfo){
    // start the thinker process
    tick(boardSHM, procInfo);

    if((waitpid (getProcChild(procInfo), NULL, 0)) < 0) {
        perror ("Error waiting for child processes to die");
        exit (EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}

int childProcess(BoardSHM* boardSHM, Connection* connection, int moveTime, ProcessInfo* procInfo){
    if (startGameLoop(connection, boardSHM, moveTime) == -1)
            printf("Terminated with Error!");
}