#include <stdio.h>

#include "communicator/prolog.h"
#include "communicator/servermessage.h"
#include "utilities.h"

void printAndFree(ServerMessage* message);

//TODO: Splitup and clean function
PlayerInfo* initiateProlog(Connection* connection, const char* version, const char* gameId, const char* playerPreference){

    ServerMessage* message = getServerGreeting(connection);
    if (message->isError == 1)
        panic(message->clearText);
    printAndFree(message);

    sendClientVersion(connection, version);

    message = getVersionResponse(connection);
    if (message->isError == 1)
        panic(message->clearText);
    printAndFree(message);

    sendGameId(connection, gameId);

    message = getGameKind(connection);
    if (message->isError == 1)
        panic(message->clearText);
    printAndFree(message);

    message = getGameName(connection);
    if (message->isError == 1)
        panic(message->clearText);
    printAndFree(message);

    formatAndSend(connection, "PLAYER", playerPreference, NULL, false);
    //sendPlayerPreference(connection, playerPreference);

    message = getPlayerMeta(connection);
    if (message->isError == 1)
        panic(message->clearText);

    Player* own = parseOwn(message);
    if(own != NULL) {

    }

    printAndFree(message);

    printf("?\n");
    //TODO: Parse and display properly
    message = getEndplayers(connection);
    if (message->isError == 1)
        panic(message->clearText);
    printAndFree(message);
    printf("??\n");

    PlayerInfo *info = malloc(sizeof(PlayerInfo));
    return info;
}

Player* parseOwn(ServerMessage* message) {
    char** data = slice(message->clearText, " ");
    int length = sliceLength(data);
    for(int i = 0; i < length; i++) {
        printf("%i > %s\n", i, data[i]);
    }

    Player* player = malloc(sizeof(Player));
    return player;
}

void printAndFree(ServerMessage* message){
    printServerMessage(message);
    freeServerMessage(message);
}