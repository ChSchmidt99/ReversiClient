#ifndef MCST_API_H
#define MCST_API_H
    
    #include <stdlib.h>
    #include <time.h>
    #include "mcst_boardstate.h"
    // Only 8x8 boards currently supported
    // Time in Seconds
    //TODO: use time in ms
    char* CalculateNextMoveAI(char(*board)[BOARD_SIZE], char forPlayer, time_t minCalculatingTime, time_t maxCalculatingTime, size_t threads);
    char* CalculateNextMoveAIOptimizedThreads(char(*board)[BOARD_SIZE], char forPlayer, time_t minCalculatingTime, time_t maxCalculatingTime);
#endif