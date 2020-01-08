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

    InputParams inputParams;
    if (initInputParams(argc,argv, &inputParams) == -1)
        panic("Failed to init input params");

    ProcessManagementInput forkIn;
    forkIn.preForkHandler = &preForkHandler;
    forkIn.communicatorEntry = &communicatorEntry;
    forkIn.thinkerEntry = &thinkerEntry;
    forkIn.inputParams = &inputParams;

    int err = startProcessManagement(&forkIn);
    deinitInputParams(&inputParams);
    exitWithExitCode(err);
}

int preForkHandler(Connection* connection, InputParams* params, InitialSharedData* initSharedDataOut){
    GameInstance* gameInstance = executeProlog(connection, params);
    if (gameInstance == NULL){
        perror("Failed Prolog\n");
        return EXIT_FAILURE;
    }

    int moveTime = waitForFirstMove(connection);
    if (moveTime == -1){
        perror("Failed to wait for first Move!\n");
        return EXIT_FAILURE;
    }

    size_t rows = 0;
    size_t cols = 0;
    if (receiveBoardDimensions(connection,&rows,&cols) == -1){
        return EXIT_FAILURE;
    }
    
    initSharedDataOut->boardSize = rows;
    initSharedDataOut->gameName = copyStringToNewMemoryAddr(gameInstance->gameName);
    initSharedDataOut->moveTime = moveTime;
    initSharedDataOut->opponentCount = gameInstance->opponentCount;
    initSharedDataOut->ownInfo = copyPlayerMetaToNewAddr(gameInstance->ownPlayer);
    if (gameInstance->opponentCount >= MAX_OPPONENTS)
        panic("Number of opponents not implemented!");
    for(size_t i = 0; i < gameInstance->opponentCount; i++)
        initSharedDataOut->opponents[i] = copyPlayerMetaToNewAddr(gameInstance->opponents[i]);

    //TODO: Memory Leak! gameInstance stuff has to be copied to initSharedDataOut
    freeGameInstance(gameInstance);
    return 0;
}

int communicatorEntry(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM){
    if (startGameLoop(connection, boardSHM, gameSHM) == -1)
        return -1;
     else
        return 1;
    return 0;
}

int thinkerEntry(BoardSHM* boardSHM, GameDataSHM* gameSHM){    
    if (initThinkerOnce(boardSHM,gameSHM) == -1){
        return -1;
    }

    ProcessInfo* processInfo = getProcessInfo(gameSHM);
    while (waitpid(processInfo->communicatorPID,NULL,0) == 0);
    freeProcessInfo(processInfo);
    
    deinitThinker();
    printf("Thinker returning\n");
    return 0;
}

GameInstance* executeProlog(Connection* connection, InputParams* params){
    printf("Starting Prolog...\n");
    GameInstance* gameInstance = initiateProlog(connection,VERSION_NUMBER,params->gameId, params->playerPreference);
    if (gameInstance == (GameInstance*)-1){
        perror("Failed Prolog!\n");
        return NULL;
    }
    printf("Succesfully Finished Prolog, Game Instance Info: \n\n");
    printGameInstanceDetails(gameInstance);
    return gameInstance;
}

int initInputParams(int argc, char* argv[], InputParams* inputParams){
    char* gameId = readGameID(argc,argv);
    if (gameId == NULL)
        panic("GameId must be set!");
    
    inputParams->gameId = gameId;
    char* playerPref = readPreferencedPlayerNumber(argc,argv);    
    //TODO: Do in a cleaner and safer way (Convert from 1 -> 0 and 2 -> 1)
    if (playerPref != NULL)
        playerPref[0] -= 1;
    inputParams->playerPreference = playerPref;
    char* filePath = getConfigPath(argc,argv);
    int ret = setParamsFromFile(inputParams,filePath);
    free(filePath);
    return ret;
}

void deinitInputParams(InputParams* inputParams){
    free(inputParams->hostName);
    free(inputParams->portNumber);
    free(inputParams->gameKind);
    free(inputParams->gameId);
    free(inputParams->playerPreference);
}

char* getConfigPath(int argc, char *argv[]){
    char* configFilePath = readConfigFilePath(argc, argv);
    if (configFilePath == NULL) 
        return copyStringToNewMemoryAddr(DEFAULT_CONFIG_PATH);
    else
        return configFilePath;
}