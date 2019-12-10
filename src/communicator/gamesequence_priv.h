#include "communicator/gamesequence.h"
#include "communicator/servermessage.h"

#define OK_WAIT_COMMAND "OKWAIT"
#define PLAY_COMMAND "PLAY"
#define BOARD_SIZE 8

void gameLoop(Connection* connection, SharedMemory* sharedMem);
ServerMessage* receiveMessage(Connection* connection);
void interpretAndFreeServerMessage(Connection* connection, ServerMessage* serverMessage, SharedMemory* sharedMem);
void receivedMove(Connection* connection, SharedMemory* sharedMem);
void receivedMoveOk(Connection* connection, SharedMemory* sharedMem);
void receivedWait(Connection* connection, SharedMemory* sharedMem);
void receivedGameover(Connection* connection, SharedMemory* sharedMem);
char* getMove();
char** receiveBoard(Connection* connection, size_t* lengthOut);
void receiveBoardDimensions(Connection* connection, int *rows, int *cols);
void sendMove(Connection* connection, char* move);
void writeBoardToSharedMemory(char** board, size_t boardSize, SharedMemory* sharedMem);
void convertBoard(char** stringBoard, size_t boardSize, char boardBuffer[][boardSize]);