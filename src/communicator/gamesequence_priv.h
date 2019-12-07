#include "communicator/servermessage.h"
#include "communicator/connection.h"

#define OK_WAIT_COMMAND "OKWAIT"
#define PLAY_COMMAND "PLAY"
#define BOARD_SIZE 8

void startGameLoop(Connection* connection);
void gameLoop(Connection* connection);
ServerMessage* receiveMessage(Connection* connection);
void interpretAndFreeServerMessage(Connection* connection, ServerMessage* serverMessage);
void receivedMove(Connection* connection);
void receivedMoveOk(Connection* connection);
void receivedWait(Connection* connection);
void receivedGameover(Connection* connection);
char* getMove();
void receiveBoard(Connection* connection, char* boardBuffer[BOARD_SIZE]);
void receiveBoardDimensions(Connection* connection, int *rows, int *cols);
void sendMove(Connection* connection, char* move);
void writeBoardToSharedMemory(char* board[BOARD_SIZE]);