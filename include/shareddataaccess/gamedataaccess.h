#ifndef GAMEDATAACCESS_H
#define GAMEDATAACCESS_H
    #include "core.h"
    #include <stdlib.h>

    typedef struct _GameDataSHM GameDataSHM;

    GameDataSHM* createGameDataSHM();
    int detachGameDataSHM(GameDataSHM* sharedMemory);
    int clearGameDataSHM(GameDataSHM* sharedMemory);
    void setGameName(GameDataSHM* shm, char* gameName);
    char* getGameName(GameDataSHM* shm);
    void setProcessIDs(GameDataSHM* shm, int PIDs[2]);
    int* getProcessIDs(GameDataSHM* shm);
    void setOwnPlayerMeta(GameDataSHM* shm, PlayerMeta* meta);
    PlayerMeta* getOwnPlayerMeta(GameDataSHM* shm);
    void setOpponenPlayerMeta(GameDataSHM* shm, PlayerMeta* meta, size_t index);
    PlayerMeta* getOpponentPlayerMeta(GameDataSHM* shm, size_t index);
    void setOpponenCount(GameDataSHM* shm, size_t count);
    size_t getOpponenCount(GameDataSHM* shm);

    

    
#endif