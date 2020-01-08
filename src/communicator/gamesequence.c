//TODO: Remove prolog references
#include "communicator/prolog.h"
#include "communicator/communicator.h"
#include "misc/utilities.h"
#include "gamesequence_priv.h"
#include <string.h>
#include <signal.h>
#include <sys/select.h>
#include <unistd.h>

#define MOVE_BUFFER_SIZE 3

int startGameLoop(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM){
    if (executeMoveSequence(connection,boardSHM, gameSHM) == -1)
        return -1;
    
    if (gameLoop(connection,boardSHM, gameSHM) == -1)
        return -1;
    
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
    //TODO: cleanup!
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
        return receivedMove(connection,boardSHM, gameSHM);
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

int receivedMove(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM){
    logMessage("Received Move Command",1);
    size_t rows;
    size_t cols;
    if (receiveBoardDimensions(connection,&rows,&cols) == -1)
        return -1;


    if (executeMoveSequence(connection,boardSHM, gameSHM) == -1)
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
    size_t rows;
    size_t cols;
    if (receiveBoardDimensions(connection,&rows,&cols) == -1)
        return -1;

    char board[rows][cols];
    if(receiveBoard(connection,rows, board) == -1)
        return -1;

    //TODO: Own function
    printf("Winning Board:\n");
    for (size_t i = 0; i < rows; i++){
        for (size_t j = 0; j < rows; j++){
            printf("%c",board[i][j]);
        }
        printf("\n");
    }

    //TODO: Get player name and print it 
    //TODO: Own function
    int player0Winner = player0Won(connection);
    player1Won(connection);
    if (player0Winner){
        printf("Player 0 Won!\n");
    } else {
        printf("Player 1 Won!\n");
    }

    return 0;
}

int writeBoardToSharedMemory(size_t boardSize, char board[][boardSize], BoardSHM* boardSHM, GameDataSHM* gameSHM){
    setBoard(boardSHM,boardSize,board);
    return 0;
}

int executeMoveSequence(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM){
    size_t boardSize = getBoardSize(boardSHM);
    char board[boardSize][boardSize];
    if(receiveBoard(connection,boardSize, board) == -1)
        return -1;
    
    for(size_t i = 0; i < boardSize; i++){
        for(size_t j = 0; j < boardSize; j++){
            printf("%c ",board[i][j]);
        }
        printf("\n");
    }

    if (writeBoardToSharedMemory(boardSize, board, boardSHM, gameSHM) == -1)
        return -1;
    
    notifyServerAboutThinking(connection);
    if (signalThinker(gameSHM) == -1)
        return -1;

    char* move = waitForThinkerResponse(connection,gameSHM);
    if (move == (char*)-1)
        return -1;

    sendMove(connection,move);
    free(move);
    return 0;
}

int notifyServerAboutThinking(Connection* connection){
    sendThinking(connection);
    if (!receiveOkThink(connection)){
        printf("Did not receive OKTHINK!\n");
        return -1;
    } else {
        return 0;
    }
}

int signalThinker(GameDataSHM* gameSHM){
    setIsThinking(gameSHM, 1);
    
    ProcessInfo* processInfo = getProcessInfo(gameSHM);
    pid_t thinkerPID = processInfo->thinkerPID;
    freeProcessInfo(processInfo);

    if (kill(thinkerPID, SIGUSR1) == -1){
        perror("Failed to signal thinker\n");
        return -1;
    }
    return 0;
}

char* waitForThinkerResponse(Connection* connection, GameDataSHM* gameSHM){    
    ProcessInfo* processInfo = getProcessInfo(gameSHM);
    int readFD = readFileDescriptor(processInfo->fd);
    freeProcessInfo(processInfo);

    int timeout = getMoveTime(gameSHM) / 1000;
    if (fileDescriptorReadIsReady(readFD,timeout)){
        char* move = safeMalloc(sizeof(char) * MOVE_BUFFER_SIZE);
        ssize_t readAmount = read(readFD,move,MOVE_BUFFER_SIZE);
        if (readAmount != MOVE_BUFFER_SIZE){
            perror("Failed to read from Pipe");
            return (char*) -1;
        }  
        return move;
    } else {
        perror("Timeout waiting for thinker\n");
        return (char*) -1;
    }
}