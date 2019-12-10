#include "boarddataaccess_priv.h"

#include "utilities.h"
#include "string.h"
#include <sys/ipc.h>
#include <sys/shm.h>

BoardSHM* newBoardSHM(int shmId, size_t boardSize, char board[]){
    BoardSHM* boardSHM = malloc(sizeof(BoardSHM));
    boardSHM->board = board;
    boardSHM->boardSize = boardSize;
    boardSHM->shmId = shmId;
    return boardSHM;
}

void freeBoardSHM(BoardSHM* shm){
    free(shm);
}

BoardSHM* createBoardSHM(size_t boardSize){
    int shmId = shmget(IPC_PRIVATE,sizeof(char) * boardSize * boardSize,IPC_CREAT | 0666);
    if (shmId == -1)
        return NULL;

    char* board = shmat(shmId,0,0);
    if (board == (char*)-1)
        return NULL;
    
    return newBoardSHM(shmId, boardSize, board);
}

int detachBoardSHM(BoardSHM* sharedMemory){
    if (shmdt(sharedMemory->board) == -1)
        return -1;
    else 
        return 0;
}

int clearBoardSHM(BoardSHM* sharedMemory){
    if (shmctl(sharedMemory->shmId,IPC_RMID,0) == -1)
        return -1;

    freeBoardSHM(sharedMemory);
    return 0;
}

void setBoard(BoardSHM* shm, size_t boardSize, char board[][boardSize]){
    if (shm->boardSize != boardSize)
        panic("BoardSize passed into setBoard did not match shm->boardSize");
    
    for(size_t i = 0; i < boardSize; i++){
        for(size_t j = 0; j < boardSize; j++){
            shm->board[j + i * boardSize] = board[i][j];
        }
    }
}
char (*getBoard(BoardSHM* shm, size_t *boardSizeOut))[]{
    *boardSizeOut = shm->boardSize;
    char (*board)[*boardSizeOut] = malloc(sizeof(char) * (*boardSizeOut) * (*boardSizeOut));
    for(size_t i = 0; i < *boardSizeOut; i++){
        for(size_t j = 0; j < *boardSizeOut; j++){
            board[i][j] = shm->board[j + i * (*boardSizeOut)];
        }
    }
    return board;
}