#include <stdio.h>
#include <stdlib.h>

#include "communicator/prolog.h"
#include "communicator/servermessage.h"
#include "utilities.h"

void printAndFree(char* message);

//TODO: Splitup and clean function
void initiateProlog(Connection* connection, const char* version, const char* gameId, const char* playerPreference){

    char* message = getServerGreeting(connection);
    printAndFree(message);

    sendClientVersion(connection, version);

    message = getVersionResponse(connection);
    printAndFree(message);

    sendGameId(connection, gameId);

    message = getGameKind(connection);
    printAndFree(message);

    message = getGameName(connection);
    printAndFree(message);

    sendPlayerPreference(connection, playerPreference);

    PlayerMeta* meta = getPlayerMeta(connection);
    printf("Player Number: %i\n", meta->number);
    printf("Player Name: %s\n",meta->name);
    freePlayerMeta(meta);

    int totalPlayers = getTotalPlayers(connection);
    for (int i = 0; i < totalPlayers - 1; i++){
        message = getOtherPlayer(connection);
        printAndFree(message);
    }

    if (!nextMessageIsEndplayers(connection))
        panic("Expected ENDPLAYERS");
}

void printAndFree(char* message){
    printf("%s\n",message);
    free(message);
}