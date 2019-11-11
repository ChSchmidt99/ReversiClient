#include "../include/optionreader.h"
#include <stdlib.h>
#include <stdio.h>

#define GAMEKINDNAME "Reversi"
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.mu.de"

int main(int argc, char *argv[]) {
    char* GameId = readGameID(argc,argv);
    if(GameId == NULL){
        printf("GameId must be set!\n");
        return EXIT_FAILURE;
    }
    char* PlayerNumberPreference = readPreferencedPlayerNumber(argc,argv);
    return EXIT_SUCCESS;
}