#include "shareddataaccess/shareddataaccess.h"

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
    char board[GAME_BOARD_SIZE * GAME_BOARD_SIZE];
} SharedDataAccess;


typedef struct _SharedMemory {
    SharedDataAccess* sharedData;
    int shmId;
} SharedMemory;

SharedMemory* newSharedMemory(SharedDataAccess* shmData, int shmId);
void freeSharedMemory(SharedMemory* shm);
void setPlayerMeta(SharedPlayerInfo* dest,PlayerMeta* src);