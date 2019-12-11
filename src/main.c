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
void teardownSHM(BoardSHM* boardSHM, GameDataSHM* gameSHM);
void initGameSHM(GameDataSHM* gameSHM, GameInstance* gameInstance);

int thinkerProcess(BoardSHM* boardSHM,GameDataSHM* gameSHM, ProcessInfo* procInfo);
int communicatorProcess(BoardSHM* boardSHM, GameDataSHM* gameSHM, Connection* connection, int moveTime, ProcessInfo* procInfo);

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
    GameDataSHM* gameSHM = createGameDataSHM();
    initGameSHM(gameSHM,gameInstance);
    freeGameInstance(gameInstance);

    pid_t parentId = getpid();
    
    ProcessInfo* processInfo = createProcessInfo();
    setProcParent(processInfo, &parentId);

    int exitCode = EXIT_SUCCESS;
    pid_t processID = fork();
    if (processID < 0){
        printf("Failed To Fork!\n");
        teardownSHM(boardSHM,gameSHM);
    } else if (processID == 0){
        //Child
        setProcChild(processInfo, &processID);
        setCommunicatorPID(gameSHM,getpid());
        exitCode = communicatorProcess(boardSHM, gameSHM, connection, moveTime, processInfo);
        teardownConnection(connection);
        kill(SIGTERM,*processInfo->parent);
    } else {
        //Parent
        teardownConnection(connection);
        setCommunicatorPID(gameSHM,getpid());
        exitCode = thinkerProcess(boardSHM, gameSHM, processInfo);
    }

    teardownSHM(boardSHM, gameSHM);
    return exitCode;
}

void initGameSHM(GameDataSHM* gameSHM, GameInstance* gameInstance){
    setGameName(gameSHM, gameInstance->gameName);
    setOwnPlayerMeta(gameSHM, gameInstance->ownPlayer);
    setOpponenCount(gameSHM, gameInstance->opponentCount);
    setIsThinking(gameSHM,0);
    for (size_t i = 0; i < gameInstance->opponentCount; i++)
        setOpponenPlayerMeta(gameSHM, gameInstance->opponents[i],i);
}

void teardownSHM(BoardSHM* boardSHM, GameDataSHM* gameSHM){
    if (detachBoardSHM(boardSHM) == -1)
        panic("Failed to detach boardSHM");
    if (clearBoardSHM(boardSHM) == -1)
        panic("Failed to clear boardSHM");

    if (detachGameDataSHM(gameSHM) == -1)
        panic("Failed to detach gameSHM");
    if (clearGameDataSHM(gameSHM) == -1)
        panic("Failed to clear gameSHM");
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

int thinkerProcess(BoardSHM* boardSHM,GameDataSHM* gameSHM, ProcessInfo* procInfo){
    
    close(readFileDescriptor(procInfo));
    
    int exitCode = EXIT_SUCCESS;

    if (startThinker(boardSHM,gameSHM,procInfo) == -1){
        exitCode = EXIT_FAILURE;
    }

    if((waitpid (getProcChild(procInfo), NULL, 0)) < 0) {
        perror ("Error waiting for child processes to die");
        exitCode = EXIT_FAILURE;
    }

    close(writeFileDescriptor(procInfo));
    return exitCode;
}

int communicatorProcess(BoardSHM* boardSHM, GameDataSHM* gameSHM, Connection* connection, int moveTime, ProcessInfo* procInfo){
    close(writeFileDescriptor(procInfo));
    int readFD = procInfo->fd[0];
    if (startGameLoop(connection, boardSHM, gameSHM, moveTime,readFD) == -1){
        close(readFileDescriptor(procInfo));
        return EXIT_FAILURE;
    } else{
        close(readFileDescriptor(procInfo));
        return EXIT_SUCCESS;
    }
}