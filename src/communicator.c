#include <string.h>
#include <stdbool.h>

#include "../include/communicator.h"
#include "../include/connection.h"
#include "../include/utilities.h"

#define ENDPLAYERS "ENDPLAYERS"
#define VERSION "VERSION"
#define GAMEID "ID"
#define PLAYERPREFERENCE "PLAYER"

//TODO: Parse Server Messages and wrap responses in ServerMessageType
//TODO: Cache if wrong response type
bool isEndPlayers(char* message);

ServerMessage* getServerGreeting(Connection* connection){
    return parseServerMessage(readServerMessage(connection));
}

ServerMessage* getVersionResponse(Connection* connection){
    return parseServerMessage(readServerMessage(connection));
}

ServerMessage* getGameKind(Connection* connection){
    return parseServerMessage(readServerMessage(connection));
}

ServerMessage* getGameName(Connection* connection){
    return parseServerMessage(readServerMessage(connection));
}

ServerMessage* getPlayerMeta(Connection* connection){
    return parseServerMessage(readServerMessage(connection));
}

List* getOtherPlayers(Connection* connection){
    List* otherPlayers = newEmptyList();

    bool receivingPlayers = true;
    while(receivingPlayers){
        char* player = readServerMessage(connection);
        if (isEndPlayers(player))
            receivingPlayers = false;
        else 
            append(otherPlayers, player);       
        
    }
    return otherPlayers;
}

bool isEndPlayers(char* message){
    if (strcmp(message, ENDPLAYERS) == 0)
        return true;
    else
        return false;    
}

void logMessage(char* message){
    printf("Sending: '%s'\n",message);
}

void sendClientVersion(Connection* connection, const char* version){
    char* message = concatStringToNewMemoryAddr(VERSION,version," ");
    logMessage(message);
    writeServerMessage(connection, message);
    free(message);
}

void sendGameId(Connection* connection, const char* gameID){
    char* message = concatStringToNewMemoryAddr(GAMEID,gameID," ");
    logMessage(message);
    writeServerMessage(connection, message);
    free(message);
}

void sendPlayerPreference(Connection* connection, const char* preference){
    if (preference == NULL){
        logMessage(PLAYERPREFERENCE);
        writeServerMessage(connection, PLAYERPREFERENCE);
    } else {
        char* message = concatStringToNewMemoryAddr(PLAYERPREFERENCE,preference," ");
        logMessage(message);
        writeServerMessage(connection, message);
        free(message);
    }
}