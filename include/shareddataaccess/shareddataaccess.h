#ifndef SHAREDBOARDACCESS_H
#define SHAREDBOARDACCESS_H
    #include "core.h"
    #include <stdlib.h>
    #define GAME_BOARD_SIZE 8

    typedef struct _SharedMemory SharedMemory;

    SharedMemory* createSharedMemory();
    int clearSharedData(SharedMemory* sharedMemory);
    
    void setGameName(SharedMemory* shm, char* gameName);
    char* getGameName(SharedMemory* shm);
    void setProcessIDs(SharedMemory* shm, int PIDs[2]);
    int* getProcessIDs(SharedMemory* shm);
    void setOwnPlayerMeta(SharedMemory* shm, PlayerMeta* meta);
    PlayerMeta* getOwnPlayerMeta(SharedMemory* shm);
    void setOpponenPlayerMeta(SharedMemory* shm, PlayerMeta* meta, size_t index);
    PlayerMeta* getOpponentPlayerMeta(SharedMemory* shm, size_t index);
    void setOpponenCount(SharedMemory* shm, size_t count);
    size_t getOpponenCount(SharedMemory* shm);
    void setBoard(SharedMemory* shm, char board[][GAME_BOARD_SIZE]);
    char (*getBoard(SharedMemory* shm))[GAME_BOARD_SIZE];

#endif