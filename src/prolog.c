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
    
    char* message = readServerMessage(connection);
    printf("%s",message);
    free(message);

    writeMessageToServer(connection,"VERSION 2.3");
    
    char* message2 = readServerMessage(connection);
    printf("%s",message2);
    free(message2);
    
    writeMessageToServer(connection,"ID 1rzjzsqhr73mv");
    
    char* message3 = readServerMessage(connection);
    printf("%s",message3);
    free(message3);

    char* message4 = readServerMessage(connection);
    printf("%s",message4);
    free(message4);

    writeMessageToServer(connection,"PLAYER");

    char* message5 = readServerMessage(connection);
    printf("%s",message5);
    free(message5);

    char* message6 = readServerMessage(connection);
    printf("%s",message6);
    free(message6);

    /*
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

    
    message = getEndplayers(connection);
    if (message->type == Error)
        die(message->clearText);
    printAndFree(message);

    
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