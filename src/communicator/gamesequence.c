#include "communicator/prolog.h"
#include "utilities.h"
#include "gamesequence_priv.h"

//TODO: Always unwrap messages and remove + before command
#define ENDFILED_COMMAND "+ ENDFIELD"

void startGameLoop(Connection* connection){
    gameLoop(connection);
}

void gameLoop(Connection* connection){
    ServerMessage* message = receiveMessage(connection);
    interpretAndFreeServerMessage(connection,message);
}

ServerMessage* receiveMessage(Connection* connection){
    char* message = malloc(sizeof(char) * DEFAULT_MESSAGE_BUFFER_SIZE);
    readServerMessage(connection, DEFAULT_MESSAGE_BUFFER_SIZE, message);
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
        printf("Got unexpected Servermessage: %s\n",serverMessage->messageReference);
    }
}

void receivedMove(Connection* connection){
    size_t boardSize = 0;
    char** board = receiveBoard(connection, &boardSize);
    writeBoardToSharedMemory(board, boardSize);
    char* move = getMove();
    sendMove(connection,move);
    free(move);
    gameLoop(connection);
}

void receivedMoveOk(Connection* connection){
    gameLoop(connection);
}

void receivedWait(Connection* connection){
    writeLineToServer(connection, OK_WAIT_COMMAND);
    gameLoop(connection);
}

void receivedGameover(Connection* connection){
    panic("Implement Me");
}

char* getMove(){
    panic("Implement Me");
    return "";
}

//Use proper array instead of double ptr
char** receiveBoard(Connection* connection, size_t* lengthOut){
    int rows;
    int cols;
    receiveBoardDimensions(connection,&rows,&cols);
    
    char** board = malloc(sizeof(char*) * rows);
    for(int i = 0; i < rows; i++){
        char* row = malloc(sizeof(char) * DEFAULT_MESSAGE_BUFFER_SIZE);
        readServerMessage(connection,DEFAULT_MESSAGE_BUFFER_SIZE,row);
        //TODO: Properly handle error
        if (row[0] == '-')
            panic(row);
        row += 2;
        //TODO: resize row arr size, because it is way to big
        board[i] = row;
    }

    char* endField = malloc(sizeof(char) * DEFAULT_MESSAGE_BUFFER_SIZE);
    readServerMessage(connection,DEFAULT_MESSAGE_BUFFER_SIZE,endField);
    
    //TODO: Properly handle error
    /*
    if (strcmp(endField,ENDFILED_COMMAND) != 0)
        panic(endField);
    */

    *lengthOut = rows;
    return board;
}

void receiveBoardDimensions(Connection* connection, int *rows, int *cols){
    char* fieldDimensions = malloc(sizeof(char) * DEFAULT_MESSAGE_BUFFER_SIZE);
    readServerMessage(connection,DEFAULT_MESSAGE_BUFFER_SIZE,fieldDimensions);
    size_t fieldDimensionTokenCount = 0;
    char** fieldDimensionTokens = slice(fieldDimensions," ",&fieldDimensionTokenCount);
    size_t dimensionCount = 0;
    char** dimensions = slice(fieldDimensionTokens[1],",",&dimensionCount);
    *cols = atoi(dimensions[0]);
    *rows = atoi(dimensions[1]);
    free(fieldDimensions);
    freeTokens(fieldDimensionTokens);
    freeTokens(dimensions);
}

void sendMove(Connection* connection, char* move){
    char* moveString = concatStringToNewMemoryAddr(PLAY_COMMAND,move," ");
    writeLineToServer(connection,moveString);
    free(moveString);
}

void writeBoardToSharedMemory(char** board, size_t boardSize){
    panic("Implement Me");
}