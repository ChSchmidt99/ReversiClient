//TODO: Remove prolog references
#include "communicator/prolog.h"
#include "communicator/communicator.h"
#include "utilities.h"
#include "gamesequence_priv.h"
#include <string.h>

void startGameLoop(Connection* connection, BoardSHM* boardSHM, int moveTime){
    logMessage("Started Game Loop",1);
    //executeMoveSequence(connection,boardSHM, moveTime);
    //gameLoop(connection,boardSHM);
}

void gameLoop(Connection* connection, BoardSHM* boardSHM){
    ServerMessage* message = receiveServerMessage(connection);
    interpretAndFreeServerMessage(connection,message, boardSHM);
}

void interpretAndFreeServerMessage(Connection* connection, ServerMessage* serverMessage, BoardSHM* boardSHM){
    //TODO: Free Server Message
    switch (serverMessage->type){
    case Error:
        printf("Gor Error: %s\n",serverMessage->messageReference);
        freeServerMessage(serverMessage);
        return;
    case Wait:
        freeServerMessage(serverMessage);
        receivedWait(connection,boardSHM);
        return;
    case Gameover:
        freeServerMessage(serverMessage);
        receivedGameover(connection,boardSHM);
        return;

    case Move:{
        int moveTime = parseMoveTime(serverMessage);
        freeServerMessage(serverMessage);
        receivedMove(connection,boardSHM, moveTime);
        return;
    }
    case MoveOk:
        freeServerMessage(serverMessage);
        receivedMoveOk(connection,boardSHM);
        return;
    case Quit:
        freeServerMessage(serverMessage);
        receivedQuit(connection,boardSHM);
    default:
        printf("Got unexpected Servermessage: %s\n",serverMessage->messageReference);
        freeServerMessage(serverMessage);
    }
}

void receivedMove(Connection* connection, BoardSHM* boardSHM, int moveTime){
    logMessage("Received Move Command",1);
    size_t rows;
    size_t cols;
    receiveBoardDimensions(connection,&rows,&cols);
    executeMoveSequence(connection,boardSHM, moveTime);
    gameLoop(connection,boardSHM);
}

void receivedQuit(Connection* connection, BoardSHM* boardSHM){
    logMessage("Quit received",1);
}

void receivedMoveOk(Connection* connection, BoardSHM* boardSHM){
    logMessage("Received MoveOk Command",1);
    gameLoop(connection,boardSHM);
}

void receivedWait(Connection* connection, BoardSHM* boardSHM){
    logMessage("Received Wait Command",1);
    sendOkWait(connection);
    gameLoop(connection,boardSHM);
}

void receivedGameover(Connection* connection, BoardSHM* boardSHM){
    logMessage("Received GameOver Command",1);
    panic("Implement Me");
}

char* getMove(){
    panic("Implement Me");
    return "";
}

void writeBoardToSharedMemory(char** board, size_t boardSize, BoardSHM* boardSHM){
    char convertedBoard[boardSize][boardSize];
    convertBoard(board,boardSize,convertedBoard);
    //setBoard(sharedMem,convertedBoard);
}

void convertBoard(char** stringBoard, size_t boardSize, char boardBuffer[][boardSize]){
    for (size_t i = 0; i < boardSize; i++){
        char* row = newStringWithoutDelimiter(stringBoard[i],' ');
        if (strlen(row) != boardSize + 1)
            panic("Unexpected board row length\n");
        for (size_t j = 0; j < boardSize; j++){
            boardBuffer[i][j] = row[j + 1];
        }
    }
}

void executeMoveSequence(Connection* connection, BoardSHM* boardSHM, int moveTime){
    printf("executeMoveSequence");
    size_t boardSize = getBoardSize(boardSHM);
    char** stringBoard = receiveBoard(connection,boardSize);
    
    printf("Time For Move: %i\n",moveTime);

    for(size_t i = 0; i < boardSize; i++){
        printf("%s\n",stringBoard[i]);
    }

    writeBoardToSharedMemory(stringBoard, boardSize, boardSHM);
    char* move = getMove();
    sendMove(connection,move);
    free(move);
}