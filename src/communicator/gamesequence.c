//TODO: Remove prolog references
#include "communicator/prolog.h"
#include "communicator/communicator.h"
#include "utilities.h"
#include "gamesequence_priv.h"
#include <string.h>
#include <signal.h>

int startGameLoop(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM, int moveTime){
    logMessage("Started Game Loop",1);
    if (executeMoveSequence(connection,boardSHM, gameSHM, moveTime) == -1){
        return -1;
    }
    if (gameLoop(connection,boardSHM, gameSHM) == -1){
        return -1;
    }
    return 0;
}

int gameLoop(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM){
    ServerMessage* message = receiveServerMessage(connection);
    if (interpretAndFreeServerMessage(connection,message, boardSHM, gameSHM) == -1)
        return -1;
    else 
        return 0;
}

int interpretAndFreeServerMessage(Connection* connection, ServerMessage* serverMessage, BoardSHM* boardSHM, GameDataSHM* gameSHM){
    //TODO: Free Server Message
    switch (serverMessage->type){
    case Error:
        printf("Gor Error: %s\n",serverMessage->messageReference);
        freeServerMessage(serverMessage);
        return -1;
    case Wait:
        freeServerMessage(serverMessage);
        return receivedWait(connection,boardSHM, gameSHM);
    case Gameover:
        freeServerMessage(serverMessage);
        return receivedGameover(connection,boardSHM, gameSHM);
    case Move:{
        int moveTime = parseMoveTime(serverMessage);
        freeServerMessage(serverMessage);
        if (moveTime == -1)
            return -1;
        return receivedMove(connection,boardSHM, gameSHM, moveTime);
    }
    case MoveOk:
        freeServerMessage(serverMessage);
        return receivedMoveOk(connection,boardSHM, gameSHM);
    case Quit:
        freeServerMessage(serverMessage);
        return receivedQuit(connection,boardSHM, gameSHM);
    default:
        printf("Got unexpected Servermessage: %s\n",serverMessage->messageReference);
        freeServerMessage(serverMessage);
        return -1;
    }
}

int receivedMove(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM, int moveTime){
    logMessage("Received Move Command",1);
    size_t rows;
    size_t cols;
    if (receiveBoardDimensions(connection,&rows,&cols) == -1)
        return -1;


    if (executeMoveSequence(connection,boardSHM, gameSHM, moveTime) == -1)
        return -1;
    else 
        return gameLoop(connection,boardSHM, gameSHM);
}

int receivedQuit(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM){
    logMessage("Quit received",1);
    return 0;
}

int receivedMoveOk(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM){
    logMessage("Received MoveOk Command",1);
    return gameLoop(connection,boardSHM, gameSHM);
}

int receivedWait(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM){
    logMessage("Received Wait Command",1);
    sendOkWait(connection);
    return gameLoop(connection,boardSHM, gameSHM);
}

int receivedGameover(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM){
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

int writeBoardToSharedMemory(char** board, size_t boardSize, BoardSHM* boardSHM, GameDataSHM* gameSHM){
    char convertedBoard[boardSize][boardSize];
    if (convertBoard(board,boardSize,convertedBoard) == -1)
        return -1;

    printf("trying to set Board...\n");
    setBoard(boardSHM,boardSize,convertedBoard);
    printf("set board successful\n");
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

int executeMoveSequence(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM, int moveTime){
    printf("executeMoveSequence");
    size_t boardSize = getBoardSize(boardSHM);
    char** stringBoard = receiveBoard(connection,boardSize);
    if (stringBoard == (char**) -1)
        return -1;
    
    printf("Time For Move: %i\n",moveTime);

    for(size_t i = 0; i < boardSize; i++){
        printf("%s\n",stringBoard[i]);
    }

    if (writeBoardToSharedMemory(stringBoard, boardSize, boardSHM, gameSHM) == -1)
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

void signalThinker(pid_t thinkerPID){
    //TODO: Add Flag to gameData SHM, which shows thinker that signal is send from correct process!    
    kill(thinkerPID, SIGUSR1);
}