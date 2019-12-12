#include "process_priv.h"

#include "misc/utilities.h"
#include <stdlib.h>
#include <unistd.h>

int startProcessManagement(ProcessManagementInput* input){
    Connection* connection = initiateConnectionSequence(input->inputParams->configParams->hostName,input->inputParams->configParams->portNumber);
    InitialSharedData initialSharedData;
    if (input->preForkHandler(connection,input->inputParams,&initialSharedData) == -1){
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
        int ret = input->communicatorEntry(processInfo,connection,boardSHM,gameSHM);
        teardownCommunicatorProcess(processInfo,connection,gameSHM,boardSHM);
        exitWithExitCode(ret);
    } else {
        ProcessInfo* processInfo = setupThinkerProcess(fd,gameSHM,connection,processID);
        int ret = input->thinkerEntry(processInfo, boardSHM, gameSHM);
        teardownThinkerProcess(processInfo,gameSHM,boardSHM);    
        return ret;
    }
    return -1;
}

ProcessInfo* setupCommunicatorProcess(int pipe[2], GameDataSHM* gameSHM){
    ProcessInfo* processInfo = createProcessInfo(pipe,getppid(),getpid());
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

ProcessInfo* setupThinkerProcess(int pipe[2], GameDataSHM* gameSHM, Connection* connection, pid_t childPID){
    ProcessInfo* processInfo = createProcessInfo(pipe,getpid(),childPID);
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

ProcessInfo* createProcessInfo(int pipe[2], pid_t parent, pid_t child){
    ProcessInfo* info = malloc(sizeof(ProcessInfo));
    info->fd[0] = pipe[0];
    info->fd[1] = pipe[1];
    info->parent = parent;
    info->child = child;
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

pid_t getChildPID(ProcessInfo* info){
    return info->child;
}

pid_t getParentPID(ProcessInfo* info){
    return info->parent;
}