#include "communicator/gamesequence.h"
#include "communicator/servermessage.h"

//#define BOARD_SIZE 8

int gameLoop(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM);
ServerMessage* receiveMessage(Connection* connection);
int interpretAndFreeServerMessage(Connection* connection, ServerMessage* serverMessage, BoardSHM* boardSHM, GameDataSHM* gameSHM);
int receivedMove(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM);
int receivedMoveOk(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM);
int receivedWait(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM);
int receivedGameover(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM);
int receivedQuit(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM);
char* waitForThinkerResponse(Connection* connection, GameDataSHM* gameSHM);

int executeMoveSequence(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM);
int writeBoardToSharedMemory(char** board, size_t boardSize, BoardSHM* boardSHM, GameDataSHM* gameSHM);
int convertBoard(char** stringBoard, size_t boardSize, char boardBuffer[][boardSize]);

int notifyServerAboutThinking(Connection* connection);
int signalThinker(GameDataSHM* gameSHM);
int pipeReadIsReady(int fd);