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
    logMessage("Started Game Loop",1);
    if (executeMoveSequence(connection,boardSHM, gameSHM) == -1){
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

//TODO: Check if quit is caught everywhere!
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

int executeMoveSequence(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM){
    size_t boardSize = getBoardSize(boardSHM);
    char** stringBoard = receiveBoard(connection,boardSize);
    if (stringBoard == (char**) -1)
        return -1;
    
    for(size_t i = 0; i < boardSize; i++){
        printf("%s\n",stringBoard[i]);
    }

    if (writeBoardToSharedMemory(stringBoard, boardSize, boardSHM, gameSHM) == -1)
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

    printf("Sending Think Signal\n");
    if (kill(thinkerPID, SIGUSR1) == -1){
        perror("Failed to signal thinker");
        return -1;
    }
    return 0;
}

char* waitForThinkerResponse(Connection* connection, GameDataSHM* gameSHM){    
    printf("waitForThinker Response...\n");

    ProcessInfo* processInfo = getProcessInfo(gameSHM);
    int readFD = readFileDescriptor(processInfo->fd);
    freeProcessInfo(processInfo);

    if (pipeReadIsReady(readFD)){
        printf("Pipe is Ready!\n");
        char* move = safeMalloc(sizeof(char) * MOVE_BUFFER_SIZE);
        ssize_t readAmount = read(readFD,move,MOVE_BUFFER_SIZE);
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