#include "main_priv.h"

#include "misc/optionreader.h"
#include "misc/config.h"
#include "thinker/thinker.h"
#include "misc/utilities.h"
#include "communicator/gamesequence.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

#define VERSION_NUMBER "2.3"
#define DEFAULT_CONFIG_PATH "./client.conf"

//TODO: Cach strc+c signal and execute teardown!
int main(int argc, char *argv[]){
    srand(time(NULL));
    char* configFilePath = getConfigPath(argc,argv);
    Params* params = getParamsFromFile(configFilePath);
    free(configFilePath);
    
    ProcessManagementInput forkIn;
    forkIn.preForkHandler = &preForkHandler;
    forkIn.communicatorEntry = &communicatorEntry;
    forkIn.thinkerEntry = &thinkerEntry;
    forkIn.params = params;

    int err = startProcessManagement(&forkIn);
    freeParams(params);
    exitWithExitCode(err);
}

int preForkHandler(Connection* connection, InitialSharedData* initSharedDataOut){
    initSharedDataOut->boardSize = 8;
    initSharedDataOut->gameName = "Test Board Name";
    initSharedDataOut->moveTime = 3000;
    initSharedDataOut->opponentCount = 1;
    
    PlayerMeta* ownInfo = newPlayerMeta(0,"Player 1",1);
    PlayerMeta* testOpponent = newPlayerMeta(1,"Player 2",1);
    
    initSharedDataOut->opponents[0] = testOpponent;
    initSharedDataOut->ownInfo = ownInfo;

    printf("Pre Fork Handler\n");
    /*
    Match* match = initMatch(argc,argv,connection);
    if (match == (Match*) -1){
        teardownConnection(connection);
        return EXIT_FAILURE;
    }
    */
    return 0;
}

int communicatorEntry(ProcessInfo* processInfo){
    printf("Communicator Entry!\n");
    //exitCode = communicatorProcess(match->boardSHM, match->gameSHM, connection, match->moveTime, processInfo);
    return 0;
}

int thinkerEntry(ProcessInfo* processInfo){
    printf("Thinker Entry!\n");
    //exitCode = thinkerProcess(match->boardSHM, match->gameSHM, processInfo);
    return 0;
}


/*

Match* initMatch(int argc, char *argv[],Connection* connection){
    char* gameId = readGameID(argc,argv);
    if (gameId == NULL)
        panic("GameId must be set!");
    
    char* playerPreference = readPreferencedPlayerNumber(argc,argv);

    GameInstance* gameInstance = initiateProlog(connection,VERSION_NUMBER,gameId, playerPreference);
    if (gameInstance == (GameInstance*)-1){
        printf("Failed Prolog!");
        
        return EXIT_FAILURE;
    }
    printGameInstanceDetails(gameInstance);
    free(gameId);
    free(playerPreference);

    //TODO: Move to better spot!
    int moveTime = waitForFirstMove(connection);
    if (moveTime == -1){
        printf("Failed to wait for first Move!");
        return EXIT_FAILURE;
    }

    size_t rows = 0;
    size_t cols = 0;
    if (receiveBoardDimensions(connection,&rows,&cols) == -1){
        return EXIT_FAILURE;
    }
    
    //TODO: Use rows and cols instead of size
    BoardSHM* boardSHM = createBoardSHM(rows);
    GameDataSHM* gameSHM = createGameDataSHM();
    initGameSHM(gameSHM,gameInstance);
    freeGameInstance(gameInstance);
}


int thinkerProcess(BoardSHM* boardSHM,GameDataSHM* gameSHM, ProcessInfo* procInfo){
    int exitCode = EXIT_SUCCESS;

    if (initThinkerOnce(boardSHM,gameSHM,procInfo) == -1){
        exitCode = EXIT_FAILURE;
    }

    while (waitpid(*procInfo->child,NULL,0) == 0);
    deinitThinker();
    printf("Returning Thinker Process!\n");
    return exitCode;
}

int communicatorProcess(BoardSHM* boardSHM, GameDataSHM* gameSHM, Connection* connection, int moveTime, ProcessInfo* procInfo){
    int readFD = procInfo->fd[0];
    if (startGameLoop(connection, boardSHM, gameSHM, moveTime,readFD) == -1){
        return EXIT_FAILURE;
    } else{
        return EXIT_SUCCESS;
    }
}
*/
char* getConfigPath(int argc, char *argv[]){
    char* configFilePath = readConfigFilePath(argc, argv);
    if (configFilePath == NULL) 
        return copyStringToNewMemoryAddr(DEFAULT_CONFIG_PATH);
    else
        return configFilePath;
}