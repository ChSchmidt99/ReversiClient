#include <string.h>
#include <stdbool.h>

#include "communicator/communicator.h"
#include "communicator/connection.h"
#include "utilities.h"

#define ENDPLAYERS "+ ENDPLAYERS\n"
#define VERSION "VERSION"
#define GAMEID "ID"
#define PLAYERPREFERENCE "PLAYER"

//TODO: Parse Server Messages and wrap responses in ServerMessageType
//TODO: Cache if wrong response type

void logMessage(char* message);

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
            panic(player);
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

void logMessage(char* message){
    printf("Sending: '%s'\n",message);
}

void send(Connection* connection, char* data, bool freeData) {
    logMessage(data);
    writeMessageToServer(connection, data);
    if(freeData)
        free(data);
}

void formatAndSend(Connection* connection, char* data, const char* firstParam, const char* secondParam, bool freeData) {
    if(firstParam == NULL && secondParam == NULL) {
        send(connection, data, freeData);
        return;
    }
    char* message;
    if(firstParam != NULL) {
        message = concatStringToNewMemoryAddr(data, firstParam, " ");
        if(freeData)
            free(data);
    } else {
        message = data;
    }

    char* finalMessage;
    if(secondParam != NULL) {
        finalMessage = concatStringToNewMemoryAddr(message, secondParam, " ");
        if(firstParam != NULL || freeData)
            free(message);
    } else {
        finalMessage = message;
    }

    send(connection, finalMessage, true);
}

ServerMessage* receive(Connection* connection) {
    return parseServerMessage(readServerMessage(connection));
}