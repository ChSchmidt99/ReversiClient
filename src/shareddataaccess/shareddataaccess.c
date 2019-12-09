#include "shareddataaccess_priv.h"
#include "utilities.h"
#include "string.h"
#include <sys/ipc.h>
#include <sys/shm.h>


SharedMemory* newSharedMemory(SharedDataAccess* shmData, int shmId){
    SharedMemory* shm = malloc(sizeof(SharedMemory));
    shm->sharedData = shmData;
    shm->shmId = shmId;
    return shm;
}

void freeSharedMemory(SharedMemory* shm){
    free(shm);
}

SharedMemory* createSharedMemory(){
    int shmId = shmget(IPC_PRIVATE,sizeof(SharedDataAccess),IPC_CREAT | 0666);
    if (shmId == -1)
        return NULL;

    SharedDataAccess* sharedData = shmat(shmId,0,0);
    if (sharedData == (SharedDataAccess*)-1)
        return NULL;
    
    return newSharedMemory(sharedData,shmId);
}

int clearSharedData(SharedMemory* sharedMemory){
    if (shmdt(sharedMemory->sharedData) == -1)
        return -1;
    if (shmctl(sharedMemory->shmId,IPC_RMID,0) == -1)
        return -1;

    freeSharedMemory(sharedMemory);
    return 0;
}

void setGameName(SharedMemory* shm, char* gameName){
    if (strlen(gameName) >= NAME_BUFFER_SIZE)
        panic("Gamename was larger than buffer size!");
    strcpy(shm->sharedData->gameName,gameName);
}

char* getGameName(SharedMemory* shm){
    return shm->sharedData->gameName;
}

void setProcessIDs(SharedMemory* shm, int PIDs[2]){
    shm->sharedData->prozessIds[0] = PIDs[0];
    shm->sharedData->prozessIds[1] = PIDs[1];
}

int* getProcessIDs(SharedMemory* shm){
    return shm->sharedData->prozessIds;
}

void setOwnPlayerMeta(SharedMemory* shm, PlayerMeta* meta){
    setPlayerMeta(&shm->sharedData->ownInfo,meta);
}

PlayerMeta* getOwnPlayerMeta(SharedMemory* shm){
    return newPlayerMeta(shm->sharedData->ownInfo.playerNumber,shm->sharedData->ownInfo.name,shm->sharedData->ownInfo.isReady);
}

void setOpponenPlayerMeta(SharedMemory* shm, PlayerMeta* meta, size_t index){
    if (index >= MAX_OPPONENTS || index >= shm->sharedData->opponentCount)
        panic("Index of Opponent PlayerMeta was out of bounds");

    setPlayerMeta(&shm->sharedData->opponents[index],meta);
}

PlayerMeta* getOpponentPlayerMeta(SharedMemory* shm, size_t index){
    return newPlayerMeta(shm->sharedData->opponents[index].playerNumber,shm->sharedData->opponents[index].name,shm->sharedData->opponents[index].isReady);
}

void setOpponenCount(SharedMemory* shm, size_t count){
    shm->sharedData->opponentCount = count;
}

size_t getOpponenCount(SharedMemory* shm){
    return shm->sharedData->opponentCount;
}

void setBoard(SharedMemory* shm, char board[][GAME_BOARD_SIZE]){
    for(size_t i = 0; i < GAME_BOARD_SIZE; i++){
        for(size_t j = 0; j < GAME_BOARD_SIZE; j++){
            shm->sharedData->board[j + i * GAME_BOARD_SIZE] = board[i][j];
        }
    }
}

char (*getBoard(SharedMemory* shm))[GAME_BOARD_SIZE]{
    char (*board)[GAME_BOARD_SIZE] = malloc(sizeof(char) * GAME_BOARD_SIZE * GAME_BOARD_SIZE);
    for(size_t i = 0; i < GAME_BOARD_SIZE; i++){
        for(size_t j = 0; j < GAME_BOARD_SIZE; j++){
            board[i][j] = shm->sharedData->board[j + i * GAME_BOARD_SIZE];
        }
    }
    return board;
}

void setPlayerMeta(SharedPlayerInfo* dest,PlayerMeta* src){
    if (strlen(src->name) > NAME_BUFFER_SIZE)
        panic("Name of PlayerMeta was bigger than buffer!");
    strcpy(dest->name, src->name);
    dest->isReady = src->isReady;
    dest->playerNumber = src->number;
}