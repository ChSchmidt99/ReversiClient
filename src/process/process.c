#include "process_priv.h"

#include "misc/utilities.h"
#include <stdlib.h>
#include <unistd.h>

int startProcessManagement(ProcessManagementInput* input){
    Connection* connection = initiateConnectionSequence(input->params->hostName,input->params->portNumber);
    InitialSharedData initialSharedData;
    if (input->preForkHandler(connection, &initialSharedData) == -1){
        teardownConnection(connection);
        return -1;
    }
    
    BoardSHM* boardSHM = createBoardSHM(initialSharedData.boardSize);
    GameDataSHM* gameSHM = initGameDataSHM(&initialSharedData);

    return pipeAndFork(input,connection,gameSHM,boardSHM);
}

Connection* initiateConnectionSequence(char* hostName, char* portNumber){
    Connection* connection = newConnection(hostName,portNumber);
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
        ProcessInfo* processInfo = setupCommunicatorProcess(fd,gameSHM);
        int ret = input->communicatorEntry(processInfo);
        teardownCommunicatorProcess(processInfo,connection,gameSHM,boardSHM);
        exitWithExitCode(ret);
    } else {
        ProcessInfo* processInfo = setupThinkerProcess(fd,gameSHM,connection);
        int ret = input->thinkerEntry(processInfo);
        teardownThinkerProcess(processInfo,gameSHM,boardSHM);    
        return ret;
    }
    return -1;
}

ProcessInfo* setupCommunicatorProcess(int pipe[2], GameDataSHM* gameSHM){
    ProcessInfo* processInfo = createProcessInfo(pipe);
    setCommunicatorPID(gameSHM,getpid());
    close(writeFileDescriptor(processInfo));
    return processInfo;
}

void teardownCommunicatorProcess(ProcessInfo* processInfo, Connection* connection, GameDataSHM* gameSHM, BoardSHM* boardSHM){
    teardownConnection(connection);
    close(readFileDescriptor(processInfo));
    detachBoardSHM(boardSHM);
    freeBoardSHM(boardSHM);
    detachGameDataSHM(gameSHM);
    freeGameDataSHM(gameSHM);
    freeProcessInfo(processInfo);
}

ProcessInfo* setupThinkerProcess(int pipe[2], GameDataSHM* gameSHM, Connection* connection){
    ProcessInfo* processInfo = createProcessInfo(pipe);
    setThinkerPID(gameSHM,getpid());
    teardownConnection(connection);
    close(readFileDescriptor(processInfo));
    return processInfo;
}

void teardownThinkerProcess(ProcessInfo* processInfo, GameDataSHM* gameSHM, BoardSHM* boardSHM){
    close(writeFileDescriptor(processInfo));
    detachBoardSHM(boardSHM);
    clearBoardSHM(boardSHM);
    detachGameDataSHM(gameSHM);
    clearGameDataSHM(gameSHM);
    freeProcessInfo(processInfo);
}

int teardownConnection(Connection* connection){
    if (disconnectFromServer(connection) == -1)
        return -1;
    freeConnection(connection);
    return 0;
}

ProcessInfo* createProcessInfo(int pipe[2]){
    ProcessInfo* info = malloc(sizeof(ProcessInfo));
    info->fd[0] = pipe[0];
    info->fd[1] = pipe[1];
    info->parent = getppid();
    info->child = getpid();
    return info;
}

void freeProcessInfo(ProcessInfo* procInfo) {
    free(procInfo);
}

int readFileDescriptor(ProcessInfo* info) {
    return info->fd[0];
}

int writeFileDescriptor(ProcessInfo* info) {
    return info->fd[1];
}