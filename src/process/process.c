#include "process_priv.h"

#include "misc/utilities.h"
#include "core.h"
#include <stdlib.h>
#include <unistd.h>

int startProcessManagement(ProcessManagementInput* input){
    Connection* connection = initiateConnectionSequence(input->inputParams->hostName,input->inputParams->portNumber);
    InitialSharedData initialSharedData;
    if (input->preForkHandler(connection,input->inputParams,&initialSharedData) == -1){
        teardownConnection(connection);
        return -1;
    }
    BoardSHM* boardSHM = createBoardSHM(initialSharedData.boardSize);
    GameDataSHM* gameSHM = initGameDataSHM(&initialSharedData);
    deinitSharedData(&initialSharedData);
    return pipeAndFork(input,connection,gameSHM,boardSHM);
}

Connection* initiateConnectionSequence(char* hostName, char* portNumber){
    Connection* connection = newConnection(hostName,portNumber,CONNECTION_TIMEOUT_IN_SEC);
    if (connectToServer(connection) == -1)
        panic("Failed to connect to server");
    return connection;
}

GameDataSHM* initGameDataSHM(InitialSharedData* initData){
    GameDataSHM* gameSHM = createGameDataSHM();
    setMoveTime(gameSHM, initData->moveTime);
    setGameName(gameSHM, initData->gameName);
    setOwnPlayerMeta(gameSHM, initData->ownInfo);
    setOpponenCount(gameSHM, initData->opponentCount);
    setIsThinking(gameSHM,0);
    for (size_t i = 0; i < initData->opponentCount; i++)
        setOpponenPlayerMeta(gameSHM, initData->opponents[i],i);
    return gameSHM;
}

int pipeAndFork(ProcessManagementInput* input, Connection* connection, GameDataSHM* gameSHM, BoardSHM* boardSHM){
    int fd[2];
    if (pipe(fd) < 0)
        return -1;
    
    pid_t processID = fork();
    if (processID < 0){
        printf("Failed To Fork!\n");
    } else if (processID == 0){
        setupCommunicatorProcess(fd,gameSHM);
        int ret = input->communicatorEntry(connection,boardSHM,gameSHM);
        teardownCommunicatorProcess(connection,gameSHM,boardSHM);
        exitWithExitCode(ret);
    } else {
        setupThinkerProcess(fd,gameSHM,connection);
        int ret = input->thinkerEntry(boardSHM, gameSHM);
        teardownThinkerProcess(gameSHM,boardSHM);    
        return ret;
    }
    return -1;
}

void setupCommunicatorProcess(int pipe[2], GameDataSHM* gameSHM){
    ProcessInfo* processInfo = newProcessInfo(pipe,getppid(),getpid());
    setProcessInfo(gameSHM,processInfo);
    freeProcessInfo(processInfo);
    close(writeFileDescriptor(pipe));
}

void teardownCommunicatorProcess(Connection* connection, GameDataSHM* gameSHM, BoardSHM* boardSHM){
    ProcessInfo* processInfo = getProcessInfo(gameSHM);
    close(readFileDescriptor(processInfo->fd));
    freeProcessInfo(processInfo);
    teardownConnection(connection);
    detachBoardSHM(boardSHM);
    freeBoardSHM(boardSHM);
    detachGameDataSHM(gameSHM);
    freeGameDataSHM(gameSHM);
}

void setupThinkerProcess(int pipe[2], GameDataSHM* gameSHM, Connection* connection){
    close(readFileDescriptor(pipe));
    teardownConnection(connection);
}

void teardownThinkerProcess(GameDataSHM* gameSHM, BoardSHM* boardSHM){
    ProcessInfo* processInfo = getProcessInfo(gameSHM);
    close(writeFileDescriptor(processInfo->fd));
    freeProcessInfo(processInfo);
    detachBoardSHM(boardSHM);
    clearBoardSHM(boardSHM);
    detachGameDataSHM(gameSHM);
    clearGameDataSHM(gameSHM);
}

int teardownConnection(Connection* connection){
    if (disconnectFromServer(connection) == -1)
        return -1;
    freeConnection(connection);
    return 0;
}

int readFileDescriptor(int fd[2]) {
    return fd[0];
}

int writeFileDescriptor(int fd[2]) {
    return fd[1];
}

void deinitSharedData(InitialSharedData *sharedData){
    free(sharedData->gameName);
    freePlayerMeta(sharedData->ownInfo);
    for (size_t i = 0; i < sharedData->opponentCount; i++){
        freePlayerMeta(sharedData->opponents[i]);
    }
}