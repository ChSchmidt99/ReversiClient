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
    initInputParams(argc,argv, &inputParams);

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
        printf("Failed Prolog!");
        return EXIT_FAILURE;
    }
    printGameInstanceDetails(gameInstance);

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

int communicatorEntry(ProcessInfo* processInfo, Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM){
    printf("Communicator Entry!\n");
    if (startGameLoop(connection, boardSHM, gameSHM, processInfo) == -1)
        return -1;
     else
        return 1;
    return 0;
}

int thinkerEntry(ProcessInfo* processInfo, BoardSHM* boardSHM, GameDataSHM* gameSHM){
    printf("Thinker Entry!\n");
    
    if (initThinkerOnce(boardSHM,gameSHM,processInfo) == -1){
        return -1;
    }

    while (waitpid(getChildPID(processInfo),NULL,0) == 0);
    
    printf("Child PID was: %i\n",getChildPID(processInfo));

    deinitThinker();
    printf("Thinker returning\n");
    return 0;
}

void initInputParams(int argc,char* argv[],InputParams* inputParams){
    char* gameId = readGameID(argc,argv);
    if (gameId == NULL)
        panic("GameId must be set!");
    
    inputParams->gameId = gameId;
    inputParams->playerPreference = readPreferencedPlayerNumber(argc,argv);
    inputParams->configParams = getConfigParams(argc,argv);
}

void deinitInputParams(InputParams* inputParams){
    freeConfigParams(inputParams->configParams);
    free(inputParams->gameId);
    free(inputParams->playerPreference);
}

ConfigParams* getConfigParams(int argc, char* argv[]){
    char* configFilePath = getConfigPath(argc,argv);
    ConfigParams* params = getParamsFromFile(configFilePath);
    free(configFilePath);
    return params;
}

char* getConfigPath(int argc, char *argv[]){
    char* configFilePath = readConfigFilePath(argc, argv);
    if (configFilePath == NULL) 
        return copyStringToNewMemoryAddr(DEFAULT_CONFIG_PATH);
    else
        return configFilePath;
}