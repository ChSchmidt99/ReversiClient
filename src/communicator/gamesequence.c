//TODO: Remove prolog references
#include "communicator/prolog.h"
#include "communicator/communicator.h"
#include "utilities.h"
#include "gamesequence_priv.h"
#include <string.h>

int startGameLoop(Connection* connection, BoardSHM* boardSHM, int moveTime){
    logMessage("Started Game Loop",1);
    if (executeMoveSequence(connection,boardSHM, moveTime) == -1){
        return -1;
    }
    if (gameLoop(connection,boardSHM) == -1){
        return -1;
    }
    return 0;
}

int gameLoop(Connection* connection, BoardSHM* boardSHM){
    ServerMessage* message = receiveServerMessage(connection);
    if (interpretAndFreeServerMessage(connection,message, boardSHM) == -1)
        return -1;
    else 
        return 0;
}

int interpretAndFreeServerMessage(Connection* connection, ServerMessage* serverMessage, BoardSHM* boardSHM){
    //TODO: Free Server Message
    switch (serverMessage->type){
    case Error:
        printf("Gor Error: %s\n",serverMessage->messageReference);
        freeServerMessage(serverMessage);
        return -1;
    case Wait:
        freeServerMessage(serverMessage);
        return receivedWait(connection,boardSHM);
    case Gameover:
        freeServerMessage(serverMessage);
        return receivedGameover(connection,boardSHM);
    case Move:{
        int moveTime = parseMoveTime(serverMessage);
        freeServerMessage(serverMessage);
        if (moveTime == -1)
            return -1;
        return receivedMove(connection,boardSHM, moveTime);
    }
    case MoveOk:
        freeServerMessage(serverMessage);
        return receivedMoveOk(connection,boardSHM);
    case Quit:
        freeServerMessage(serverMessage);
        return receivedQuit(connection,boardSHM);
    default:
        printf("Got unexpected Servermessage: %s\n",serverMessage->messageReference);
        freeServerMessage(serverMessage);
        return -1;
    }
}

int receivedMove(Connection* connection, BoardSHM* boardSHM, int moveTime){
    logMessage("Received Move Command",1);
    size_t rows;
    size_t cols;
    if (receiveBoardDimensions(connection,&rows,&cols) == -1)
        return -1;


    if (executeMoveSequence(connection,boardSHM, moveTime) == -1)
        return -1;
    else 
        return gameLoop(connection,boardSHM);
}

int receivedQuit(Connection* connection, BoardSHM* boardSHM){
    logMessage("Quit received",1);
    return 0;
}

int receivedMoveOk(Connection* connection, BoardSHM* boardSHM){
    logMessage("Received MoveOk Command",1);
    return gameLoop(connection,boardSHM);
}

int receivedWait(Connection* connection, BoardSHM* boardSHM){
    logMessage("Received Wait Command",1);
    sendOkWait(connection);
    return gameLoop(connection,boardSHM);
}

int receivedGameover(Connection* connection, BoardSHM* boardSHM){
    logMessage("Received GameOver Command",1);
    panic("Implement Me");
    return 0;
}

char* getMove(){
    //TODO: Implement Me
    char* move = malloc(sizeof(char) * 3);
    move[0] = 'C';
    move[1] = '5';
    move[2] = '\0';
    return move;
}

int writeBoardToSharedMemory(char** board, size_t boardSize, BoardSHM* boardSHM){
    char convertedBoard[boardSize][boardSize];
    if (convertBoard(board,boardSize,convertedBoard) == -1)
        return -1;
    //setBoard(sharedMem,convertedBoard);
    return 0;
}

int convertBoard(char** stringBoard, size_t boardSize, char boardBuffer[][boardSize]){
    for (size_t i = 0; i < boardSize; i++){
        char* row = newStringWithoutDelimiter(stringBoard[i],' ');
        if (strlen(row) != boardSize + 1){
            printf("Unexpected board row length\n");
            return -1;
        }
        for (size_t j = 0; j < boardSize; j++){
            boardBuffer[i][j] = row[j + 1];
        }
    }
    return 0;
}

int executeMoveSequence(Connection* connection, BoardSHM* boardSHM, int moveTime){
    printf("executeMoveSequence");
    size_t boardSize = getBoardSize(boardSHM);
    char** stringBoard = receiveBoard(connection,boardSize);
    
    printf("Time For Move: %i\n",moveTime);

    for(size_t i = 0; i < boardSize; i++){
        printf("%s\n",stringBoard[i]);
    }

    if (writeBoardToSharedMemory(stringBoard, boardSize, boardSHM) == -1)
        return -1;
    
    sendThinking(connection);
    if (!receiveOkThink(connection)){
        printf("Did not receive OKTHINK!\n");
        return -1;
    }
    
    char* move = getMove();
    sendMove(connection,move);
    free(move);
    return 0;
}