#include <string.h>
#include <stdbool.h>

#include "../include/communicator.h"
#include "../include/connection.h"
#include "../include/utilities.h"

#define ENDPLAYERS "ENDPLAYERS"
#define VERSION "VERSION"
#define GAMEID "ID"
#define PLAYERPREFERENCE "PLAYER"

//TODO: Maybe Wrap responses in ServerMessageType
//TODO: Cache if wrong response type
bool isEndPlayers(char* message);

char*  getServerGreeting(Connection* connection){
    return readServerMessage(connection);
}

char*  getVersionResponse(Connection* connection){
    return readServerMessage(connection);
}

char*  getGameKind(Connection* connection){
    return readServerMessage(connection);
}

char*  getGameName(Connection* connection){
    return readServerMessage(connection);
}

char*  getPlayerMeta(Connection* connection){
    return readServerMessage(connection);
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

void sendClientVersion(Connection* connection, const char* version){
    char* message = concatStringToNewMemoryAddr(VERSION,version," ");
    //TODO: Better logging    
    printf("Sending: %s\n",message);
    writeServerMessage(connection, message, strlen(message));
    free(message);
}

void sendGameId(Connection* connection, const char* gameID){
    char* message = concatStringToNewMemoryAddr(GAMEID,gameID," ");
    //TODO: Better logging    
    printf("Sending: %s\n",message);
    writeServerMessage(connection, message, strlen(message));
    free(message);
}

void sendPlayerPreference(Connection* connection, const char* preference){
    if (preference == NULL){
        //TODO: Better logging    
        printf("Sending: %s\n",PLAYERPREFERENCE);
        writeServerMessage(connection, PLAYERPREFERENCE, strlen(PLAYERPREFERENCE));
    } else {
        char* message = concatStringToNewMemoryAddr(PLAYERPREFERENCE,preference," ");
        writeServerMessage(connection, message, strlen(message));
        free(message);
    }
}