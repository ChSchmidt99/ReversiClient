#ifndef SHAREDBOARDACCESS_H
#define SHAREDBOARDACCESS_H

    #define GAME_NAME_BUFFER_SIZE 256

    typedef struct {
        char gameName[GAME_NAME_BUFFER_SIZE];
        int playerNumber;
        int numberOfPlayers;
        int prozessIds[2];
    } SharedData;

    typedef struct _SharedMemory SharedMemory;

    SharedMemory* createSharedMemory();
    SharedData* initSharedMemory(SharedMemory* sharedMemory, char* gameName, int playerNumber, int numberOfPlayers, int prozessIds[2]);
    
    int clearSharedData(SharedMemory* sharedMemory);
    
    void setGameName(SharedData* data, char* gameName);
#endif