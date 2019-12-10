#include "communicator/gamesequence.h"
#include "communicator/servermessage.h"

//#define BOARD_SIZE 8

void gameLoop(Connection* connection, BoardSHM* boardSHM);
ServerMessage* receiveMessage(Connection* connection);
void interpretAndFreeServerMessage(Connection* connection, ServerMessage* serverMessage, BoardSHM* boardSHM);
void receivedMove(Connection* connection, BoardSHM* boardSHM);
void receivedMoveOk(Connection* connection, BoardSHM* boardSHM);
void receivedWait(Connection* connection, BoardSHM* boardSHM);
void receivedGameover(Connection* connection, BoardSHM* boardSHM);
void receivedQuit(Connection* connection, BoardSHM* boardSHM);
char* getMove();

void executeMoveSequence(Connection* connection, BoardSHM* boardSHM);

void writeBoardToSharedMemory(char** board, size_t boardSize, BoardSHM* boardSHM);
void convertBoard(char** stringBoard, size_t boardSize, char boardBuffer[][boardSize]);