#include <stdio.h>

#include "../include/prolog.h"
#include "../include/servermessage.h"
#include "../include/utilities.h"

void printAndFree(void* message);

//TODO: Splitup and clean function
void initiateProlog(Connection* connection, const char* version, const char* gameId, const char* playerPreference){
    
    ServerMessage* message = getServerGreeting(connection);
    if (message->type == Error)
        die(message->clearText);
    printAndFree(message);

    sendClientVersion(connection, version);

    message = getVersionResponse(connection);
    if (message->type == Error)
        die(message->clearText);
    printAndFree(message);

    sendGameId(connection, gameId);

    message = getGameKind(connection);
    if (message->type == Error)
        die(message->clearText);
    printAndFree(message);

    message = getGameName(connection);
    if (message->type == Error)
        die(message->clearText);
    printAndFree(message);

    sendPlayerPreference(connection, playerPreference);

    message = getPlayerMeta(connection);
    if (message->type == Error)
        die(message->clearText);
    printAndFree(message);

    List* otherPlayers = getOtherPlayers(connection);
    iterateOverList(otherPlayers, printAndFree);    
    freeList(otherPlayers);
}

void printAndFree(void* message){
    printServerMessage(message);
    freeServerMessage(message);
}