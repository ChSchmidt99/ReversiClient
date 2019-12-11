#include "communicator/gamesequence.h"
#include "communicator/servermessage.h"

//#define BOARD_SIZE 8

int gameLoop(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM, int pipeReadFD);
ServerMessage* receiveMessage(Connection* connection);
int interpretAndFreeServerMessage(Connection* connection, ServerMessage* serverMessage, BoardSHM* boardSHM, GameDataSHM* gameSHM, int pipeReadFD);
int receivedMove(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM, int moveTime, int pipeReadFD);
int receivedMoveOk(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM, int pipeReadFD);
int receivedWait(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM, int pipeReadFD);
int receivedGameover(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM, int pipeReadFD);
int receivedQuit(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM, int pipeReadFD);
char* waitForThinkerResponse(Connection* connection, int pipeReadFD);

int executeMoveSequence(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM, int moveTime, int pipeReadFD);
int writeBoardToSharedMemory(char** board, size_t boardSize, BoardSHM* boardSHM, GameDataSHM* gameSHM, int pipeReadFD);
int convertBoard(char** stringBoard, size_t boardSize, char boardBuffer[][boardSize]);

int notifyServerAboutThinking(Connection* connection);
int signalThinker(GameDataSHM* gameSHM);
int pipeReadIsReady(int fd);