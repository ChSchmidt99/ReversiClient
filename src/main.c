#include "../include/optionreader.h"
#include "../include/connector.h"
#include <stdlib.h>
#include <stdio.h>

#define GAMEKINDNAME "Reversi"
#define PORTNUMBER "1357"
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"

int main(int argc, char *argv[]) {
    char* GameId = readGameID(argc,argv);
    if (GameId == NULL) {
        printf("GameId must be set!\n");
        return EXIT_FAILURE;
    }
    char* PlayerNumberPreference = readPreferencedPlayerNumber(argc,argv);
    Connector* connector = newConnector(HOSTNAME,PORTNUMBER);
    connectToServer(connector);
    initiateProlog(connector);
    disconnectFromServer(connector);
    freeConnector(connector);
    return EXIT_SUCCESS;
}