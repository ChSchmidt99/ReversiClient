#include "ai/mcst_boardstate.h"

size_t** getPossibleMoves(char(*board)[BOARD_SIZE], char forPlayer, size_t* numberOfMovesOut);
char* getIdentifier(int row, int col);
void getPositionForIdentifier(char* identifier, int *rowOut, int *colOut);

int isAdjointMove(char(*board)[BOARD_SIZE], int row, int col);
int isOccupied(char(*board)[BOARD_SIZE], int row, int col);
int positionIsOnBoard(int row, int col);
int enclosesOpponentPiece(char(*board)[BOARD_SIZE],char playerAtTurn, int row, int col);
int enclosesOpponentPieceInDirection(char(*board)[BOARD_SIZE],char playerAtTurn, int row, int col, int deltaRow, int deltaCol);

void turnOpponentPiecesInDirection(char(*board)[BOARD_SIZE],char playerAtTurn, int row, int col, int deltaRow, int deltaCol);