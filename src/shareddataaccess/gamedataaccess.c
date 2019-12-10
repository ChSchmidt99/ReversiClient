#include "gamedataaccess_priv.h"
#include "utilities.h"
#include "string.h"
#include <sys/ipc.h>
#include <sys/shm.h>

GameDataSHM* newGameDataSHM(SharedGameData* shmData, int shmId){
    GameDataSHM* shm = malloc(sizeof(GameDataSHM));
    shm->sharedData = shmData;
    shm->shmId = shmId;
    return shm;
}

void freeGameDataSHM(GameDataSHM* shm){
    free(shm);
}

GameDataSHM* createGameDataSHM(){
    int shmId = shmget(IPC_PRIVATE,sizeof(SharedGameData),IPC_CREAT | 0666);
    if (shmId == -1)
        return NULL;

    SharedGameData* sharedData = shmat(shmId,0,0);
    if (sharedData == (SharedGameData*)-1)
        return NULL;
    
    return newGameDataSHM(sharedData,shmId);
}

int detachGameDataSHM(GameDataSHM* sharedMemory){
    if (shmdt(sharedMemory->sharedData) == -1)
        return -1;
    else 
        return 0;
}

int clearGameDataSHM(GameDataSHM* sharedMemory){
    if (shmctl(sharedMemory->shmId,IPC_RMID,0) == -1)
        return -1;

    freeGameDataSHM(sharedMemory);
    return 0;
}

void setGameName(GameDataSHM* shm, char* gameName){
    if (strlen(gameName) >= NAME_BUFFER_SIZE)
        panic("Gamename was larger than buffer size!");
    strcpy(shm->sharedData->gameName,gameName);
}

char* getGameName(GameDataSHM* shm){
    return shm->sharedData->gameName;
}

void setProcessIDs(GameDataSHM* shm, int PIDs[2]){
    shm->sharedData->prozessIds[0] = PIDs[0];
    shm->sharedData->prozessIds[1] = PIDs[1];
}

int* getProcessIDs(GameDataSHM* shm){
    return shm->sharedData->prozessIds;
}

void setOwnPlayerMeta(GameDataSHM* shm, PlayerMeta* meta){
    setPlayerMeta(&shm->sharedData->ownInfo,meta);
}

PlayerMeta* getOwnPlayerMeta(GameDataSHM* shm){
    return newPlayerMeta(shm->sharedData->ownInfo.playerNumber,shm->sharedData->ownInfo.name,shm->sharedData->ownInfo.isReady);
}

void setOpponenPlayerMeta(GameDataSHM* shm, PlayerMeta* meta, size_t index){
    if (index >= MAX_OPPONENTS || index >= shm->sharedData->opponentCount)
        panic("Index of Opponent PlayerMeta was out of bounds");

    setPlayerMeta(&shm->sharedData->opponents[index],meta);
}

PlayerMeta* getOpponentPlayerMeta(GameDataSHM* shm, size_t index){
    return newPlayerMeta(shm->sharedData->opponents[index].playerNumber,shm->sharedData->opponents[index].name,shm->sharedData->opponents[index].isReady);
}

void setOpponenCount(GameDataSHM* shm, size_t count){
    shm->sharedData->opponentCount = count;
}

size_t getOpponenCount(GameDataSHM* shm){
    return shm->sharedData->opponentCount;
}

void setPlayerMeta(SharedPlayerInfo* dest,PlayerMeta* src){
    if (strlen(src->name) > NAME_BUFFER_SIZE)
        panic("Name of PlayerMeta was bigger than buffer!");
    strcpy(dest->name, src->name);
    dest->isReady = src->isReady;
    dest->playerNumber = src->number;
}