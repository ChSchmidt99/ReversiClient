#include "communicator/gamesequence.h"
#include "communicator/servermessage.h"

#define OK_WAIT_COMMAND "OKWAIT"
#define PLAY_COMMAND "PLAY"
#define BOARD_SIZE 8

void gameLoop(Connection* connection, GameDataSHM* sharedMem);
ServerMessage* receiveMessage(Connection* connection);
void interpretAndFreeServerMessage(Connection* connection, ServerMessage* serverMessage, GameDataSHM* sharedMem);
void receivedMove(Connection* connection, GameDataSHM* sharedMem);
void receivedMoveOk(Connection* connection, GameDataSHM* sharedMem);
void receivedWait(Connection* connection, GameDataSHM* sharedMem);
void receivedGameover(Connection* connection, GameDataSHM* sharedMem);
char* getMove();
char** receiveBoard(Connection* connection, size_t* lengthOut);
void receiveBoardDimensions(Connection* connection, int *rows, int *cols);
void sendMove(Connection* connection, char* move);
void writeBoardToSharedMemory(char** board, size_t boardSize, GameDataSHM* sharedMem);
void convertBoard(char** stringBoard, size_t boardSize, char boardBuffer[][boardSize]);