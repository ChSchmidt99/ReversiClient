#include "misc/minunit.h"
#include "shareddataaccess/boarddataaccess.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

#define BOARD_SIZE_SHM 8

int boardsAreEqual(size_t boardSize, char board1[][boardSize],char board2[][boardSize]);

static char* test_set_and_get_board(){
    BoardSHM* sharedMem = createBoardSHM(BOARD_SIZE_SHM);
    char testBoard[][BOARD_SIZE_SHM] = {
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
        setBoard(sharedMem,BOARD_SIZE_SHM,testBoard);
        detachBoardSHM(sharedMem);
        exit(EXIT_SUCCESS);
    } else {
        //Parent
        wait(NULL);
        char (*board)[BOARD_SIZE_SHM] = getBoard(sharedMem);
        mu_assert("test_set_and_get_board failed, boards were not equal",boardsAreEqual(BOARD_SIZE_SHM,testBoard,board));
        free(board);
        detachBoardSHM(sharedMem);
    }
    clearBoardSHM(sharedMem);
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

static char * board_data_access_tests() {
    mu_run_test(test_set_and_get_board);
    return 0;
}
 