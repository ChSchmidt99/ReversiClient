#include "optionreader.h"
#include "communicator/connection.h"
#include "communicator/prolog.h"
#include <stdlib.h>
#include <stdio.h>

#define GAMEKINDNAME "Reversi"
#define PORTNUMBER "1357"
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define VERSION_NUMBER "2.3"

int main(int argc, char *argv[]) {
    char* gameId = readGameID(argc,argv);
    if (gameId == NULL) {
        printf("GameId must be set!\n");
        return EXIT_FAILURE;
    }
    char* playerPreference = readPreferencedPlayerNumber(argc,argv);
    
    Connection* connection = newConnection(HOSTNAME,PORTNUMBER);
    connectToServer(connection);
    initiateProlog(connection,VERSION_NUMBER,gameId, playerPreference);
    disconnectFromServer(connection);
    freeConnection(connection);
    free(gameId);
    free(playerPreference);
    return EXIT_SUCCESS;
}