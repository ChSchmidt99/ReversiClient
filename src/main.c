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


#define VERSION_NUMBER "2.3"
#define DEFAULT_CONFIG_PATH "./client.conf"

Connection* initiateConnectionSequence(int argc, char *argv[]);
void teardownConnection(Connection* connection);

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
    if (receiveBoardDimensions(connection,&rows,&cols) != -1){
        teardownConnection(connection);
        return EXIT_FAILURE;
    }
    
    //TODO: Use rows and cols instead of size
    BoardSHM* boardSHM = createBoardSHM(rows);

    if (startGameLoop(connection, boardSHM, moveTime) == -1)
        printf("Terminated with Error!");

    //pid_t processID;
    /*
    if((processID = fork()) < 0) {
        panic("Failed to fork");
    } else if (processID == 0){
        //Parend
        wait(NULL);
    } else {
        //Child
        startGameLoop(connection, boardSHM, moveTime);
        disconnectFromServer(connection);
        freeConnection(connection);
        free(gameId);
        free(playerPreference);
        exit(EXIT_SUCCESS);
    }
    */

    disconnectFromServer(connection);
    freeConnection(connection);
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