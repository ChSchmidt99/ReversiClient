#include <string.h>
#include <stdbool.h>

#include "../include/communicator.h"
#include "../include/connection.h"
#include "../include/utilities.h"

#define ENDPLAYERS "+ ENDPLAYERS\n"
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

char** getOtherPlayers(Connection* connection, int n){
    //TODO: Mode parsing to Server Message
    char** names = malloc(sizeof(char*)*n);
    for (int i = 0; i < n; i++){
        char* player = readServerMessage(connection);
        if (*player == '+'){
            printf("Got Player: %s",player);
            names[i] = player; 
        } else {
            die(player);
        }
    }
    return names;
}

int getTotalPlayers(Connection* connection){
    //TODO: Wrap result in Servermessage
    ServerMessage* message = parseServerMessage(readServerMessage(connection));
    if (message->type == Error)
        return -1;

    //TODO: Properly parse number in parse Server Message
    char ASCInum = message->clearText[6];
    return ASCInum - '0';
}

ServerMessage* getEndplayers(Connection* connection){
    return parseServerMessage(readServerMessage(connection));
}

void logMessage(char* message){
    printf("Sending: '%s'\n",message);
}

void sendClientVersion(Connection* connection, const char* version){
    char* message = concatStringToNewMemoryAddr(VERSION,version," ");
    logMessage(message);
    writeMessageToServer(connection, message);
    free(message);
}

void sendGameId(Connection* connection, const char* gameID){
    char* message = concatStringToNewMemoryAddr(GAMEID,gameID," ");
    logMessage(message);
    writeMessageToServer(connection, message);
    free(message);
}

void sendPlayerPreference(Connection* connection, const char* preference){
    if (preference == NULL){
        logMessage(PLAYERPREFERENCE);
        writeMessageToServer(connection, PLAYERPREFERENCE);
    } else {
        char* message = concatStringToNewMemoryAddr(PLAYERPREFERENCE,preference," ");
        logMessage(message);
        writeMessageToServer(connection, message);
        free(message);
    }
}