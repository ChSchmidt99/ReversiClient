#include "mcst_boardstate_priv.h"
#include "misc/utilities.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_POSSIBLE_MOVES 64

char (*NewEmptyBoard())[BOARD_SIZE]{
    char(*board)[BOARD_SIZE] = malloc(sizeof(char) * BOARD_SIZE * BOARD_SIZE);
    
    //TODO: Remove NULL checks for performance?
    if (board == NULL)
        panic("Malloc Failed");

    for (int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){
            board[i][j] = MCST_EMPTY_SYMBOL;
        }
    }
    return board;
}

void InitBoardWithStartingPosition(char(*board)[BOARD_SIZE]){
    board[3][3] = MCST_PLAYER_1_SYMBOL;
    board[3][4] = MCST_PLAYER_2_SYMBOL;
    board[4][3] = MCST_PLAYER_2_SYMBOL;
    board[4][4] = MCST_PLAYER_1_SYMBOL;
}

char (*CopyBoard(char(*board)[BOARD_SIZE]))[BOARD_SIZE]{
    char(*out)[BOARD_SIZE] = malloc(sizeof(char) * BOARD_SIZE * BOARD_SIZE);
    if (out == NULL)
        panic("Malloc Failed");

    for (size_t i = 0; i < BOARD_SIZE; i++){
        for (size_t j = 0; j < BOARD_SIZE; j++){
            out[i][j] = board[i][j];
        }
    } 
    return out;
}

void FreeBoard(char(*board)[BOARD_SIZE]){
    free(board);
}

List* GetPossibleBoardStates(char(*board)[BOARD_SIZE], char forPlayer){
    size_t numberOfStates;
    size_t** possibleMoves = getPossibleMoves(board,forPlayer,&numberOfStates);
    List* states = newEmptyList();
    for (size_t i = 0; i < numberOfStates; i++){
        char (*state)[BOARD_SIZE] = CopyBoard(board);
        int possibleMoveRow = possibleMoves[i][0];
        int possibleMoveCol = possibleMoves[i][1];
        ExecuteMove(state,possibleMoveRow,possibleMoveCol,forPlayer);
        free(possibleMoves[i]);
        append(states,state);
    }
    free(possibleMoves);
    return states;
}

char (*GetRandomPossibleBoardState(char(*board)[BOARD_SIZE], char forPlayer))[BOARD_SIZE]{
    size_t moveCount;
    size_t** possibleMoves = getPossibleMoves(board,forPlayer,&moveCount);
    int theChosenMove = getRandomInt(0,moveCount);
    char(*out)[BOARD_SIZE] = CopyBoard(board);
    int moveRow = possibleMoves[theChosenMove][0];
    int moveCol = possibleMoves[theChosenMove][1];
    ExecuteMove(out,moveRow,moveCol,forPlayer);
    for (size_t i = 0; i < moveCount; i++){
        free(possibleMoves[i]);
    }
    free(possibleMoves);
    return out;
}

//TODO: Remove duplicate code
size_t GetPossibleMoveCount(char(*board)[BOARD_SIZE], char forPlayer){
    size_t moveCount = 0;
    for (size_t row = 0; row < BOARD_SIZE; row++){
        for (size_t col = 0; col < BOARD_SIZE; col++){
            if (isOccupied(board,row,col) == 0){
                if (isAdjointMove(board,row,col) == 1){
                    if (enclosesOpponentPiece(board, forPlayer, row, col) == 1){
                        moveCount++;
                    }
                }
            }
        }
    }
    return moveCount;
}

int GameIsFinished(char(*board)[BOARD_SIZE]){
    size_t moveCountPlayer1 = GetPossibleMoveCount(board,MCST_PLAYER_1_SYMBOL);
    if (moveCountPlayer1 > 0)
        return 0;
    size_t moveCountPlayer2 = GetPossibleMoveCount(board,MCST_PLAYER_2_SYMBOL);
    if (moveCountPlayer2 > 0)
        return 0;
    else 
        return 1;
}

char GetWinner(char(*board)[BOARD_SIZE]){
    int player1count = 0;
    int player2count = 0;
    for(size_t row = 0; row < BOARD_SIZE; row++){
        for(size_t col = 0; col < BOARD_SIZE; col++){
            if (board[row][col] == MCST_PLAYER_1_SYMBOL){
                player1count++;
            } else if (board[row][col] == MCST_PLAYER_2_SYMBOL) {
                player2count++;
            }
        }   
    }
    if (player1count < player2count){
        return MCST_PLAYER_2_SYMBOL;
    } else if (player1count > player2count){
        return MCST_PLAYER_1_SYMBOL;
    } else {
        return MCST_EMPTY_SYMBOL;
    }
}

char GetNextPlayer(char(*currentBoardState)[BOARD_SIZE],char currentPlayer){
    char otherPlayer = GetOtherPlayer(currentPlayer);
    size_t moveCount = GetPossibleMoveCount(currentBoardState,otherPlayer);
    if (moveCount > 0)
        return otherPlayer;
    else 
        return currentPlayer;
}

char* GetMoveForBoardStates(char(*initialBoard)[BOARD_SIZE], char(*boardAfterMove)[BOARD_SIZE]){
    for (size_t row = 0; row < BOARD_SIZE; row++){
        for (size_t col = 0; col < BOARD_SIZE; col++){
            if (initialBoard[row][col] == MCST_EMPTY_SYMBOL && boardAfterMove[row][col] != MCST_EMPTY_SYMBOL){
                return getIdentifier(row,col);
            }
        }   
    }
    return "";
}

void PrintBoard(char(*board)[BOARD_SIZE]){
    printf("\n");
    for (int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){
            printf("%c",board[i][j]);
        }
        printf("\n");
    }
}

void ExecuteMoveWithIdentifier(char(*board)[BOARD_SIZE], char* move, char forPlayer){
    int row;
    int col;
    getPositionForIdentifier(move,&row,&col);
    for(int deltaRow = -1; deltaRow <= 1; deltaRow++){
        for(int deltaCol = -1; deltaCol <= 1; deltaCol++){
            if (deltaRow != 0 || deltaCol != 0){
                turnOpponentPiecesInDirection(board,forPlayer,row,col,deltaRow,deltaCol);
            }
        }   
    }
    board[row][col] = forPlayer;
}


void ExecuteMove(char(*board)[BOARD_SIZE], int row, int col, char forPlayer){
    for(int deltaRow = -1; deltaRow <= 1; deltaRow++){
        for(int deltaCol = -1; deltaCol <= 1; deltaCol++){
            if (deltaRow != 0 || deltaCol != 0){
                turnOpponentPiecesInDirection(board,forPlayer,row,col,deltaRow,deltaCol);
            }
        }   
    }
    board[row][col] = forPlayer;
}

size_t** getPossibleMoves(char(*board)[BOARD_SIZE], char forPlayer, size_t* numberOfMovesOut){
    size_t index = 0;
    size_t** out = malloc(sizeof(size_t*) * MAX_POSSIBLE_MOVES);
    if (out == NULL)
        panic("Malloc Failed");

    for (size_t row = 0; row < BOARD_SIZE; row++){
        for (size_t col = 0; col < BOARD_SIZE; col++){
            if (isOccupied(board,row,col) == 0){
                if (isAdjointMove(board,row,col) == 1){
                    if (enclosesOpponentPiece(board, forPlayer, row, col) == 1){
                        size_t* arr = malloc(sizeof(size_t) * 2);
                        if (board == NULL)
                            panic("Malloc Failed");
                        arr[0] = row;
                        arr[1] = col;
                        out[index] = arr;
                        index ++;
                    }
                }
            }
        }
    }
    *numberOfMovesOut = index;
    return out;
}

//TODO: Only 8x8 Boards supportded, make dynamic!
char* getIdentifier(int row, int col){
    char* out = malloc(sizeof(char) * 3);
    if (out == NULL)
        panic("Malloc Failed");

    out[0] = 'A' + col;
    out[1] = '8' - row;
    out[2] = '\0';
    return out;
}

void getPositionForIdentifier(char* identifier, int *rowOut, int *colOut){
    *rowOut = identifier[1] - '1';
    *colOut = identifier[0] - 'A';
}

int isAdjointMove(char(*board)[BOARD_SIZE], int row, int col){
    for (int i = -1; i < 2; i++){
        for (int j = -1; j < 2; j++){
            if (i != 0 || j != 0){
                int checkedRow = row + i;
                int checkedCol = col + j;
                if (positionIsOnBoard(checkedRow,checkedCol) == 1){
                    if (isOccupied(board,checkedRow,checkedCol) == 1){
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

//TODO: check if perfomance might increase by removing functions
int isOccupied(char(*board)[BOARD_SIZE], int row, int col){
    if (board[row][col] != MCST_EMPTY_SYMBOL){
        return 1;
    } else {
        return 0;
    }
}

int positionIsOnBoard(int row, int col){
    if (row < 0 || col < 0 || row >= BOARD_SIZE || col >= BOARD_SIZE){
        return 0;
    } else {
        return 1;
    }
}

int enclosesOpponentPiece(char(*board)[BOARD_SIZE],char playerAtTurn, int row, int col){
    for(int deltaRow = -1; deltaRow <= 1; deltaRow++){
        for(int deltaCol = -1; deltaCol <= 1; deltaCol++){
            if (deltaRow != 0 || deltaCol != 0){
                if (enclosesOpponentPieceInDirection(board,playerAtTurn,row,col,deltaRow,deltaCol) == 1){
                    return 1;
                }
            }
        }   
    }
    return 0;
}

int enclosesOpponentPieceInDirection(char(*board)[BOARD_SIZE],char playerAtTurn, int row, int col, int deltaRow, int deltaCol){
    int passedOverEnemyPiece = 0;
    for(int i = 0; i < BOARD_SIZE; i++){
        row += deltaRow;
        col += deltaCol;

        if (positionIsOnBoard(row,col) == 0)
            return 0;
        
        
        if (board[row][col] == MCST_EMPTY_SYMBOL)
            return 0;
        else if (board[row][col] != playerAtTurn)
            passedOverEnemyPiece = 1;
        else 
            return passedOverEnemyPiece;
        
    }
    return 0;
}

void turnOpponentPiecesInDirection(char(*board)[BOARD_SIZE],char playerAtTurn, int row, int col, int deltaRow, int deltaCol){
    if (enclosesOpponentPieceInDirection(board,playerAtTurn,row,col,deltaRow,deltaCol)){
        for(size_t i = 0; i < BOARD_SIZE; i++){
            row += deltaRow;
            col += deltaCol;

            if (positionIsOnBoard(row,col) == 0){
                perror("unexpectedly reached end of board");
                return;
            }
            
            if (board[row][col] == MCST_EMPTY_SYMBOL){
                perror("unexpectedly reached empty field");
                return;
            } else if (board[row][col] != playerAtTurn){
                board[row][col] = playerAtTurn;
            } else {
                return;
            }
        }
    }
}

char GetOtherPlayer(char currentPlayer){
    switch (currentPlayer){
    case MCST_PLAYER_1_SYMBOL:
            return MCST_PLAYER_2_SYMBOL;
    case MCST_PLAYER_2_SYMBOL:
            return MCST_PLAYER_1_SYMBOL;
    }
    return MCST_EMPTY_SYMBOL;
}