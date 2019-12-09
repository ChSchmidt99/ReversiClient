#include "communicator/prolog.h"
#include "utilities.h"
#include "gamesequence_priv.h"

void startGameLoop(Connection* connection){
    logMessage("Started Game Loop",1);
    gameLoop(connection);
}

void gameLoop(Connection* connection){
    ServerMessage* message = receiveServerMessage(connection);
    interpretAndFreeServerMessage(connection,message);
}

void interpretAndFreeServerMessage(Connection* connection, ServerMessage* serverMessage){
    switch (serverMessage->type){
    case Error:
        printf("Gor Error: %s\n",serverMessage->messageReference);
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
    logMessage("Received Move Command",1);
    
    size_t boardSize = 0;
    char** board = receiveBoard(connection, &boardSize);

    printf("Received Board:\n");
    for (size_t i = 0; i < boardSize; i++)
        printf("%s\n",board[i]);

    writeBoardToSharedMemory(board, boardSize);
    char* move = getMove();
    sendMove(connection,move);
    free(move);
    gameLoop(connection);
}

void receivedMoveOk(Connection* connection){
    logMessage("Received MoveOk Command",1);
    gameLoop(connection);
}

void receivedWait(Connection* connection){
    logMessage("Received Wait Command",1);
    writeLineToServer(connection, OK_WAIT_COMMAND);
    gameLoop(connection);
}

void receivedGameover(Connection* connection){
    logMessage("Received GameOver Command",1);
    panic("Implement Me");
}

char* getMove(){
    panic("Implement Me");
    return "";
}

//Use proper array instead of double ptr
char** receiveBoard(Connection* connection, size_t* lengthOut){
    logMessage("Receiving Board...\n",1);
    
    int rows;
    int cols;
    receiveBoardDimensions(connection,&rows,&cols);
    char** board = malloc(sizeof(char*) * rows);
    for(int i = 0; i < rows; i++){
        //TODO: Replace receiveServerMessage with safelyReceiveServerMessage, which unwraps Error
        ServerMessage* message = receiveServerMessage(connection);
        if(message->type == Error)
            panic(message->messageReference);
            
        board[i] = copyStringToNewMemoryAddr(message->messageReference + 2);
        freeServerMessage(message);
    }

    ServerMessage* message = receiveServerMessage(connection);
    if(message->type == Error)
        panic(message->messageReference);
    else if(message->type != Endfield)
        panic("Expected Endfield!");

    freeServerMessage(message);
    *lengthOut = rows;

    logMessage("Received Board\n",1);
    return board;
}

void receiveBoardDimensions(Connection* connection, int *rows, int *cols){
    ServerMessage* message = receiveServerMessage(connection);
    if(message->type == Error)
        panic(message->messageReference);
    size_t fieldDimensionTokenCount = 0;
    char** fieldDimensionTokens = slice(message->messageReference + 2," ",&fieldDimensionTokenCount);
    size_t dimensionCount = 0;
    char** dimensions = slice(fieldDimensionTokens[1],",",&dimensionCount);
    *cols = atoi(dimensions[0]);
    *rows = atoi(dimensions[1]);
    freeServerMessage(message);
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