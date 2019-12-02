#include "minunit.h"
#include "shareddataaccess/shareddataaccess.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define TEST_GAME_NAME_STRING "Test Game Name"
#define TEST_PLAYER_NUMBER 1
#define TEST_NUM_OF_PLAYERS 2

static char * test_initSharedMemory() {
    int pIDs[2] = {1,2};
    SharedMemory* sharedMem = createSharedMemory();
    SharedData* data = initSharedMemory(sharedMem,TEST_GAME_NAME_STRING,TEST_PLAYER_NUMBER,TEST_NUM_OF_PLAYERS,pIDs);
    if (sharedMem == NULL)
        panic("Failed to init shared memory!");

    int pID = fork();
    if (pID < 0){
        panic("Failed to fork");
    } else if (pID == 0){
        //Child
        mu_assert("Failed to load from shared memory", strcmp(TEST_GAME_NAME_STRING,data->gameName) == 0);
        mu_assert("Failed to load from shared memory", TEST_NUM_OF_PLAYERS == data->numberOfPlayers);
        mu_assert("Failed to load from shared memory", TEST_PLAYER_NUMBER == data->playerNumber);
        exit(EXIT_SUCCESS);
    } else {
        mu_assert("Failed to load from shared memory", strcmp(TEST_GAME_NAME_STRING,data->gameName) == 0);
        mu_assert("Failed to load from shared memory", TEST_NUM_OF_PLAYERS == data->numberOfPlayers);
        mu_assert("Failed to load from shared memory", TEST_PLAYER_NUMBER == data->playerNumber);
    }
    clearSharedData(sharedMem);
    return 0;
}

static char * shared_memory_tests() {
    mu_run_test(test_initSharedMemory);
    return 0;
}
 