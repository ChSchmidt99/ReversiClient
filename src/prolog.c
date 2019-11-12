#include <stdio.h>

#include "../include/prolog.h"
#include "../include/servermessage.h"

void printAndFree(void* message);

//TODO: Splitup and clean function
void initiateProlog(Connection* connection, const char* version, const char* gameId, const char* playerPreference){
    ServerMessage* message = getServerGreeting(connection);
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

    message = getPlayerMeta(connection);
    printAndFree(message);

    List* otherPlayers = getOtherPlayers(connection);
    iterateOverList(otherPlayers, printAndFree);    
    freeList(otherPlayers);
}

void printAndFree(void* message){
    printServerMessage(message);
    freeServerMessage(message);
}