#include "communicator/gamesequence.h"
#include "communicator/servermessage.h"

//#define BOARD_SIZE 8

int gameLoop(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM);
ServerMessage* receiveMessage(Connection* connection);
int interpretAndFreeServerMessage(Connection* connection, ServerMessage* serverMessage, BoardSHM* boardSHM, GameDataSHM* gameSHM);
int receivedMove(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM, int moveTime);
int receivedMoveOk(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM);
int receivedWait(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM);
int receivedGameover(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM);
int receivedQuit(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM);
char* getMove();

int executeMoveSequence(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM, int moveTime);
int writeBoardToSharedMemory(char** board, size_t boardSize, BoardSHM* boardSHM, GameDataSHM* gameSHM);
int convertBoard(char** stringBoard, size_t boardSize, char boardBuffer[][boardSize]);

void signalThinker(pid_t thinkerPID);