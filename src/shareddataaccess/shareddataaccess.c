#include "shareddataaccess_priv.h"
#include "utilities.h"
#include "string.h"
#include <sys/ipc.h>
#include <sys/shm.h>


SharedMemory* newSharedMemory(SharedData* shmData, int shmId){
    SharedMemory* shm = malloc(sizeof(SharedMemory));
    shm->sharedData = shmData;
    shm->shmId = shmId;
    return shm;
}

void freeSharedMemory(SharedMemory* shm){
    free(shm);
}

SharedMemory* createSharedMemory(){
    int shmId = shmget(IPC_PRIVATE,sizeof(SharedData),IPC_CREAT | 0666);
    if (shmId == -1)
        return NULL;

    SharedData* sharedData = shmat(shmId,0,0);
    if (sharedData == (SharedData*)-1)
        return NULL;
    
    return newSharedMemory(sharedData,shmId);
}

SharedData* initSharedMemory(SharedMemory* sharedMemory, char* gameName, int playerNumber, int numberOfPlayers, int prozessIds[2]){
    if (strlen(gameName) >= GAME_NAME_BUFFER_SIZE)
        panic("Gamename was larger than buffer size!");
    strcpy(sharedMemory->sharedData->gameName,gameName);
    sharedMemory->sharedData->playerNumber = playerNumber;
    sharedMemory->sharedData->numberOfPlayers = numberOfPlayers;
    sharedMemory->sharedData->prozessIds[0] = prozessIds[0];
    sharedMemory->sharedData->prozessIds[1] = prozessIds[1];
    return sharedMemory->sharedData;
}

void setGameName(SharedData* data, char* gameName){
    if (strlen(gameName) >= GAME_NAME_BUFFER_SIZE)
        panic("Gamename was larger than buffer size!");
    strcpy(data->gameName,gameName);
}

int clearSharedData(SharedMemory* sharedMemory){
    if (shmdt(sharedMemory->sharedData) == -1)
        return -1;
    if (shmctl(sharedMemory->shmId,IPC_RMID,0) == -1)
        return -1;

    freeSharedMemory(sharedMemory);
    return 0;
}