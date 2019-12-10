#include "minunit.h"
#include "shareddataaccess/shareddataaccess.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

#define TEST_GAME_NAME_STRING "Test Game Name"
#define TEST_PLAYER_NUMBER 1
#define TEST_NUM_OF_PLAYERS 2

int boardsAreEqual(size_t boardSize, char board1[][boardSize],char board2[][boardSize]);

static char* test_set_and_get_board(){
    SharedMemory* sharedMem = createSharedMemory();
    
    char testBoard[][8] = {
        {'*','*','*','*','*','*','*','*'},
        {'*','W','*','*','B','*','*','*'},
        {'*','*','B','W','W','W','*','*'},
        {'*','*','B','W','W','W','*','*'},
        {'*','*','*','B','W','W','*','*'},
        {'*','*','*','*','B','B','*','*'},
        {'*','*','*','*','*','*','*','*'},
        {'*','*','*','*','*','*','*','*'}
    };

    int pID = fork();
    if (pID < 0){
        panic("Failed to fork");
    } else if (pID == 0){
        //Child
        setBoard(sharedMem,testBoard);
        exit(EXIT_SUCCESS);
    } else {
        //Parent
        wait(NULL);
        char (*board)[8] = getBoard(sharedMem);
        mu_assert("test_set_and_get_board failed",boardsAreEqual(8,testBoard,board));
    }
    clearSharedData(sharedMem);
    return 0;
}

int boardsAreEqual(size_t boardSize, char board1[][boardSize],char board2[][boardSize]){
    for (size_t i = 0; i < boardSize; i++){
        for (size_t j = 0; j < boardSize; j++){
            if (board1[i][j] != board2[i][j])
                return 0;
        }
    }
    return 1;
}

static char * shared_memory_tests() {
    mu_run_test(test_set_and_get_board);
    return 0;
}
 