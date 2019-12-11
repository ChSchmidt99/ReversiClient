#ifndef BOARDDATAACCESS_H
#define BOARDDATAACCESS_H
    
    #include "core.h"
    #include <stdlib.h>

    typedef struct _BoardSHM BoardSHM;
    BoardSHM* createBoardSHM(size_t boardSize);
    int detachBoardSHM(BoardSHM* sharedMemory);
    int clearBoardSHM(BoardSHM* sharedMemory);
    void setBoard(BoardSHM* shm, size_t boardSize, char board[][boardSize]);
    char (*getBoard(BoardSHM* shm))[];
    size_t getBoardSize(BoardSHM* shm);

#endif