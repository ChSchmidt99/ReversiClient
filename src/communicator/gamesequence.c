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

int startGameLoop(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM, ProcessInfo* processinfo){
    logMessage("Started Game Loop",1);
    //TODO: Pass in Process info instead of fd
    //TODO: Remove Movetime and fetch where needed
    if (executeMoveSequence(connection,boardSHM, gameSHM, getMoveTime(gameSHM), readFileDescriptor(processinfo)) == -1){
        return -1;
    }
    if (gameLoop(connection,boardSHM, gameSHM, readFileDescriptor(processinfo)) == -1){
        return -1;
    }
    return 0;
}

int gameLoop(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM, int pipeReadFD){
    ServerMessage* message = receiveServerMessage(connection);
    if (interpretAndFreeServerMessage(connection,message, boardSHM, gameSHM, pipeReadFD) == -1)
        return -1;
    else 
        return 0;
}

int interpretAndFreeServerMessage(Connection* connection, ServerMessage* serverMessage, BoardSHM* boardSHM, GameDataSHM* gameSHM, int pipeReadFD){
    //TODO: Free Server Message
    switch (serverMessage->type){
    case Error:
        printf("Gor Error: %s\n",serverMessage->messageReference);
        freeServerMessage(serverMessage);
        return -1;
    case Wait:
        freeServerMessage(serverMessage);
        return receivedWait(connection,boardSHM, gameSHM, pipeReadFD);
    case Gameover:
        freeServerMessage(serverMessage);
        return receivedGameover(connection,boardSHM, gameSHM, pipeReadFD);
    case Move:{
        int moveTime = parseMoveTime(serverMessage);
        freeServerMessage(serverMessage);
        if (moveTime == -1)
            return -1;
        return receivedMove(connection,boardSHM, gameSHM, moveTime, pipeReadFD);
    }
    case MoveOk:
        freeServerMessage(serverMessage);
        return receivedMoveOk(connection,boardSHM, gameSHM, pipeReadFD);
    case Quit:
        freeServerMessage(serverMessage);
        return receivedQuit(connection,boardSHM, gameSHM, pipeReadFD);
    default:
        printf("Got unexpected Servermessage: %s\n",serverMessage->messageReference);
        freeServerMessage(serverMessage);
        return -1;
    }
}

int receivedMove(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM, int moveTime, int pipeReadFD){
    logMessage("Received Move Command",1);
    size_t rows;
    size_t cols;
    if (receiveBoardDimensions(connection,&rows,&cols) == -1)
        return -1;


    if (executeMoveSequence(connection,boardSHM, gameSHM, moveTime, pipeReadFD) == -1)
        return -1;
    else 
        return gameLoop(connection,boardSHM, gameSHM, pipeReadFD);
}

//TODO: Check if quit is caught everywhere!
int receivedQuit(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM, int pipeReadFD){
    logMessage("Quit received",1);
    return 0;
}

int receivedMoveOk(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM, int pipeReadFD){
    logMessage("Received MoveOk Command",1);
    return gameLoop(connection,boardSHM, gameSHM, pipeReadFD);
}

int receivedWait(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM, int pipeReadFD){
    logMessage("Received Wait Command",1);
    sendOkWait(connection);
    return gameLoop(connection,boardSHM, gameSHM, pipeReadFD);
}

int receivedGameover(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM, int pipeReadFD){
    logMessage("Received GameOver Command",1);
    panic("Implement Me");
    return 0;
}

int writeBoardToSharedMemory(char** board, size_t boardSize, BoardSHM* boardSHM, GameDataSHM* gameSHM, int pipeReadFD){
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

int executeMoveSequence(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM, int moveTime, int pipeReadFD){
    size_t boardSize = getBoardSize(boardSHM);
    char** stringBoard = receiveBoard(connection,boardSize);
    if (stringBoard == (char**) -1)
        return -1;
    
    for(size_t i = 0; i < boardSize; i++){
        printf("%s\n",stringBoard[i]);
    }

    if (writeBoardToSharedMemory(stringBoard, boardSize, boardSHM, gameSHM, pipeReadFD) == -1)
        return -1;
    
    notifyServerAboutThinking(connection);
    if (signalThinker(gameSHM) == -1)
        return -1;

    char* move = waitForThinkerResponse(connection, pipeReadFD);
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
    pid_t thinkerPID = getThinkerPID(gameSHM);
    
    printf("Thinker PID was: %i\n",thinkerPID);

    printf("Sending Think Signal\n");
    if (kill(thinkerPID, SIGUSR1) == -1){
        perror("Failed to signal thinker");
        return -1;
    }
    return 0;
}

char* waitForThinkerResponse(Connection* connection, int pipeReadFD){    
    printf("waitForThinker Response...\n");
    if (pipeReadIsReady(pipeReadFD)){
        printf("Pipe is Ready!\n");
        char* move = safeMalloc(sizeof(char) * MOVE_BUFFER_SIZE);
        ssize_t readAmount = read(pipeReadFD,move,MOVE_BUFFER_SIZE);
        if (readAmount != MOVE_BUFFER_SIZE){
            printf("Failed to read from Pipe");
            return (char*) -1;
        }  
        return move;
    } else {
        printf("Wait For Thinker Timeout\n");
        return (char*) -1;
    }
}

//TODO: Pass timeout time in
int pipeReadIsReady(int fd){
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(fd,&rfds);
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    int ret = select(fd+1,&rfds,NULL,NULL,&timeout);

    printf("Select returned: %i\n",ret);

    if (ret == -1)
        printf("Select Failed!");
    
    return ret;
}