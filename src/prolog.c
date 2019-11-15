#include <stdio.h>

#include "../include/prolog.h"
#include "../include/servermessage.h"
#include "../include/utilities.h"

//TODO: remove me 
#include <string.h>
#include "../include/connection.h"

void printAndFree(ServerMessage* message);

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

    //TODO: Parse and display properly
    message = getEndplayers(connection);
    if (message->type == Error)
        die(message->clearText);
    printAndFree(message);

    /*
    int totalPlayers = getTotalPlayers(connection);
    printf("Got Total Players: %i\n",totalPlayers);
    
    char** otherPlayers = getOtherPlayers(connection, totalPlayers - 1);
    printf("Got Other Players\n");
    for (int i = 0; i < totalPlayers;i++){
        printf("%s\n",otherPlayers[i]);
        free(otherPlayers[i]);
    }
    free(otherPlayers);

    message = getEndplayers(connection);
    if (message->type == Error)
        die(message->clearText);
    printAndFree(message);
    */

}

void printAndFree(ServerMessage* message){
    printServerMessage(message);
    freeServerMessage(message);
}