#include "shareddataaccess/gamedataaccess.h"

//TODO: Make dynamic
#define NAME_BUFFER_SIZE 256
#define MAX_OPPONENTS 1

typedef struct {
    char name[NAME_BUFFER_SIZE];
    int isReady;
    int playerNumber;
} SharedPlayerInfo;

typedef struct {
    char gameName[NAME_BUFFER_SIZE];
    int prozessIds[2];
    SharedPlayerInfo ownInfo;
    size_t opponentCount;
    SharedPlayerInfo opponents[MAX_OPPONENTS];
} SharedGameData;


typedef struct _GameDataSHM {
    SharedGameData* sharedData;
    int shmId;
} GameDataSHM;

GameDataSHM* newGameDataSHM(SharedGameData* shmData, int shmId);
void freeGameDataSHM(GameDataSHM* shm);
void setPlayerMeta(SharedPlayerInfo* dest,PlayerMeta* src);