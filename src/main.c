#include "optionreader.h"
#include "config.h"
#include "communicator/connection.h"
#include "communicator/prolog.h"
#include <stdlib.h>
#include <stdio.h>

#define VERSION_NUMBER "2.3"
#define DEFAULT_CONFIG_PATH "./client.conf"

int main(int argc, char *argv[]) {
    char* configFilePath = readConfigFilePath(argc, argv);
    if (configFilePath == NULL) {
        configFilePath = DEFAULT_CONFIG_PATH;
    }
    Params* params = getParamsFromFile(configFilePath);

    char* gameId = readGameID(argc,argv);
    if (gameId == NULL) {
        printf("GameId must be set!\n");
        return EXIT_FAILURE;
    }
    char* playerPreference = readPreferencedPlayerNumber(argc,argv);
    
    Connection* connection = newConnection(params->hostName,params->portNumber);
    connectToServer(connection);
    initiateProlog(connection,VERSION_NUMBER,gameId, playerPreference);
    disconnectFromServer(connection);
    freeConnection(connection);
    free(gameId);
    free(playerPreference);
    return EXIT_SUCCESS;
}