#ifndef MCST_BOARDSTATE_H
#define MCST_BOARDSTATE_H

    #include <stdlib.h>
    #include "misc/list.h"
    #define BOARD_SIZE 8
    #define MCST_PLAYER_1_SYMBOL 'B'
    #define MCST_PLAYER_2_SYMBOL 'W'
    #define MCST_EMPTY_SYMBOL '*'

    char (*NewEmptyBoard())[BOARD_SIZE];
    void InitBoardWithStartingPosition(char(*board)[BOARD_SIZE]);
    char (*CopyBoard(char(*board)[BOARD_SIZE]))[BOARD_SIZE];
    void FreeBoard(char(*board)[BOARD_SIZE]);

    void ExecuteMove(char(*board)[BOARD_SIZE], int row, int col, char forPlayer);
    void ExecuteMoveWithIdentifier(char(*board)[BOARD_SIZE], char* move, char forPlayer);
    List* GetPossibleBoardStates(char(*board)[BOARD_SIZE], char forPlayer);
    char (*GetRandomPossibleBoardState(char(*board)[BOARD_SIZE], char forPlayer))[BOARD_SIZE];
    size_t GetPossibleMoveCount(char(*board)[BOARD_SIZE], char forPlayer);

    int GameIsFinished(char(*board)[BOARD_SIZE]);
    char GetWinner(char(*board)[BOARD_SIZE]);
    char GetNextPlayer(char(*board)[BOARD_SIZE],char currentPlayer);
    char* GetMoveForBoardStates(char(*initialBoard)[BOARD_SIZE], char(*boardAfterMove)[BOARD_SIZE]);
    void PrintBoard(char(*board)[BOARD_SIZE]);

    char GetOtherPlayer(char currentPlayer);
#endif