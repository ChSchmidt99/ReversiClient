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

#define VERSION_NUMBER "2.3"
#define DEFAULT_CONFIG_PATH "./client.conf"

//TODO: Improve Logging!
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
    GameInstance* gameInstance = initiateProlog(connection,VERSION_NUMBER,params->gameId, params->playerPreference);
    if (gameInstance == (GameInstance*)-1){
        perror("Failed Prolog!\n");
        return EXIT_FAILURE;
    }
    printGameInstanceDetails(gameInstance);

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
    initSharedDataOut->gameName = gameInstance->gameName;
    initSharedDataOut->moveTime = moveTime;
    initSharedDataOut->opponentCount = gameInstance->opponentCount;
    initSharedDataOut->ownInfo = gameInstance->ownPlayer;
    if (gameInstance->opponentCount >= MAX_OPPONENTS)
        panic("Number of opponents not implemented!");
    for(size_t i = 0; i < gameInstance->opponentCount; i++)
        initSharedDataOut->opponents[i] = gameInstance->opponents[i];

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