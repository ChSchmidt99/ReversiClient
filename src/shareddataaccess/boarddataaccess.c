#include "boarddataaccess_priv.h"

#include "misc/utilities.h"
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
        panic("Failed to get SHM");

    char* board = shmat(shmId,0,0);
    if (board == (char*)-1)
        panic("Failed to attach shm");
    
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
char (*getBoard(BoardSHM* shm))[]{
    char (*board)[shm->boardSize] = malloc(sizeof(char) * (shm->boardSize) * (shm->boardSize));
    for(size_t i = 0; i < shm->boardSize; i++){
        for(size_t j = 0; j < shm->boardSize; j++){
            board[i][j] = shm->board[j + i * (shm->boardSize)];
        }
    }
    return board;
}

size_t getBoardSize(BoardSHM* shm){
    return shm->boardSize;
}