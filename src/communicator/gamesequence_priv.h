#include "communicator/gamesequence.h"
#include "communicator/servermessage.h"

//#define BOARD_SIZE 8

int gameLoop(Connection* connection, BoardSHM* boardSHM);
ServerMessage* receiveMessage(Connection* connection);
int interpretAndFreeServerMessage(Connection* connection, ServerMessage* serverMessage, BoardSHM* boardSHM);
int receivedMove(Connection* connection, BoardSHM* boardSHM, int moveTime);
int receivedMoveOk(Connection* connection, BoardSHM* boardSHM);
int receivedWait(Connection* connection, BoardSHM* boardSHM);
int receivedGameover(Connection* connection, BoardSHM* boardSHM);
int receivedQuit(Connection* connection, BoardSHM* boardSHM);
char* getMove();

int executeMoveSequence(Connection* connection, BoardSHM* boardSHM, int moveTime);

int writeBoardToSharedMemory(char** board, size_t boardSize, BoardSHM* boardSHM);
int convertBoard(char** stringBoard, size_t boardSize, char boardBuffer[][boardSize]);