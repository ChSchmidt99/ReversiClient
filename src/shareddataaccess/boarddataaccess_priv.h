#include "shareddataaccess/boarddataaccess.h"

typedef struct _BoardSHM {
    int shmId;
    size_t boardSize;
    char* board;
} BoardSHM;

BoardSHM* newBoardSHM(int shmId, size_t boardSize, char board[]);