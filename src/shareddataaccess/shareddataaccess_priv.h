#include "shareddataaccess/shareddataaccess.h"

typedef struct _SharedMemory {
    SharedData* sharedData;
    int shmId;
} SharedMemory;

SharedMemory* newSharedMemory(SharedData* shmData, int shmId);
void freeSharedMemory(SharedMemory* shm);
