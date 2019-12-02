#include <stdio.h>

#include "communicator/prolog.h"
#include "communicator/servermessage.h"
#include "utilities.h"

//TODO: remove me 
//#include <string.h>
//#include "communicator/connection.h"

void printAndFree(ServerMessage* message);

//TODO: Splitup and clean function
void initiateProlog(Connection* connection, const char* version, const char* gameId, const char* playerPreference){

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


    sendPlayerPreference(connection, playerPreference);

    message = getPlayerMeta(connection);
    if (message->isError == 1)
        panic(message->clearText);
    printAndFree(message);

    //TODO: Parse and display properly
    message = getEndplayers(connection);
    if (message->isError == 1)
        panic(message->clearText);
    printAndFree(message);
    printf("--- TEST ---\n");
}

void printAndFree(ServerMessage* message){
    printServerMessage(message);
    freeServerMessage(message);
}