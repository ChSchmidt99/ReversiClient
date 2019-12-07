#include "communicator/prolog.h"
#include "utilities.h"
#include "gamesequence_priv.h"

void startGameLoop(Connection* connection){
    gameLoop(connection);
}

void gameLoop(Connection* connection){
    ServerMessage* message = receiveMessage(connection);
    interpretAndFreeServerMessage(connection,message);
}

ServerMessage* receiveMessage(Connection* connection){
    char* message = readServerMessage(connection);
    return parseServerMessage(message);
}

void interpretAndFreeServerMessage(Connection* connection, ServerMessage* serverMessage){
    switch (serverMessage->type){
    case Error:
        printf("Gor Error: %s\n",serverMessage->clearText);
        return;
    case Wait:
        receivedWait(connection);
        return;

    case Gameover:
        receivedGameover(connection);
        return;

    case Move:
        //TODO: Get Move Time
        receivedMove(connection);
        return;

    case MoveOk:
        receivedMoveOk(connection);
        return;
    default:
        printf("Got unexpected Servermessage: %s\n",serverMessage->message);
    }
}

void receivedMove(Connection* connection){
    char* board[BOARD_SIZE];
    receiveBoard(connection, board);
    writeBoardToSharedMemory(board);
    char* move = getMove();
    sendMove(connection,move);
    free(move);
    gameLoop(connection);
}

void receivedMoveOk(Connection* connection){
    gameLoop(connection);
}

void receivedWait(Connection* connection){
    writeMessageToServer(connection, OK_WAIT_COMMAND);
    gameLoop(connection);
}

void receivedGameover(Connection* connection){
    panic("Implement Me");
}

char* getMove(){
    panic("Implement Me");
    return "";
}

void receiveBoard(Connection* connection, char* boardBuffer[BOARD_SIZE]){
    int rows;
    int cols;
    receiveBoardDimensions(connection,&rows,&cols);
}

void receiveBoardDimensions(Connection* connection, int *rows, int *cols){
    //char* fieldDimensions = readServerMessage(connection);
    //char* fieldDimensions = "FIELD 3,5\n";
    size_t length1 = 0;
    char** slicedDimensions = slice("FIELD 3,5\n"," ",&length1);
    size_t length2 = 0;
    char** dimensions = slice(slicedDimensions[1],",",&length2);
    *cols = atoi(dimensions[0]);
    *rows = atoi(dimensions[1]);
    //free(fieldDimensions);
    free(slicedDimensions);
    free(dimensions);
}

void sendMove(Connection* connection, char* move){
    char* moveString = concatStringToNewMemoryAddr(PLAY_COMMAND,move," ");
    writeMessageToServer(connection,moveString);
    free(moveString);
}

void writeBoardToSharedMemory(char* board[BOARD_SIZE]){
    panic("Implement Me");
}