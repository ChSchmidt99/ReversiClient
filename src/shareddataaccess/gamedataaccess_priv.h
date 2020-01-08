#include "shareddataaccess/gamedataaccess.h"
#include "core.h"

//IMPROVEMENT: Make dynamic
#define NAME_BUFFER_SIZE 256

typedef struct {
    char name[NAME_BUFFER_SIZE];
    int isReady;
    int playerNumber;
} SharedPlayerInfo;

typedef struct {
    char gameName[NAME_BUFFER_SIZE];
    ProcessInfo processInfo;
    int isThinking;
    int moveTime;
    SharedPlayerInfo ownInfo;
    size_t opponentCount;
    SharedPlayerInfo opponents[MAX_OPPONENTS];
} SharedGameData;

typedef struct _GameDataSHM {
    SharedGameData* sharedData;
    int shmId;
} GameDataSHM;

GameDataSHM* newGameDataSHM(SharedGameData* shmData, int shmId);
void setPlayerMeta(SharedPlayerInfo* dest,PlayerMeta* src);