#include <string.h>
#include <stdbool.h>

#include "communicator/communicator.h"
#include "communicator/connection.h"
#include "utilities.h"

#define ENDPLAYERS_COMMAND "+ ENDPLAYERS"
#define VERSION "VERSION"
#define GAMEID "ID"
#define PLAYERPREFERENCE "PLAYER"

//TODO: Parse Server Messages and wrap responses in ServerMessageType
//TODO: Cache if wrong response type
//TODO: use readLine instead of readMessage

void logMessage(char* message);
PlayerMeta* newPlayerMeta(int number, char* name);
PlayerMeta* parsePlayerMeta(char* message);
char* getNameFromPlayerMetaTokens(char** tokens, size_t tokenCount);

char* getServerGreeting(Connection* connection){
    ServerMessage* message = receiveServerMessage(connection);
    if (message->type == Error)
        panic(message->clearText);
    
    //TODO: Use Cleartext once imlemented
    char* out = copyStringToNewMemoryAddr(message->messageReference); 
    freeServerMessage(message);
    return out;
}

char* getVersionResponse(Connection* connection){
    ServerMessage* message = receiveServerMessage(connection);
    if (message->type == Error)
        panic(message->clearText);
    //TODO: Use Cleartext once imlemented
    char* out = copyStringToNewMemoryAddr(message->messageReference);
    freeServerMessage(message);
    return out;
}

char* getGameKind(Connection* connection){
    ServerMessage* message = receiveServerMessage(connection);
    if (message->type == Error)
        panic(message->clearText);
    //TODO: Use Cleartext once imlemented
    char* out = copyStringToNewMemoryAddr(message->messageReference);
    freeServerMessage(message);
    return out;
}

char* getGameName(Connection* connection){
    ServerMessage* message = receiveServerMessage(connection);
    if (message->type == Error)
        panic(message->clearText);
    //TODO: Use Cleartext once imlemented
    char* out = copyStringToNewMemoryAddr(message->messageReference);
    freeServerMessage(message);
    return out;
}

PlayerMeta* getPlayerMeta(Connection* connection){
    ServerMessage* message = receiveServerMessage(connection);
    if (message->type == Error)
        panic(message->clearText);
    //TODO: Use Cleartext once imlemented
    PlayerMeta* out = parsePlayerMeta(message->messageReference);
    freeServerMessage(message);
    return out;
}

PlayerMeta* parsePlayerMeta(char* message){
    size_t length = 0;
    char** tokens = slice(message," ",&length);
    //TODO: find better way to do this!
    printf("Message to parse: %s\n",message);
    printf("Tokens: %zu\n",length);

    if (length < 4)
        panic("unexpected playermeta string, expected 4 tokens");

    int number = atoi(tokens[2]);
    char* name = getNameFromPlayerMetaTokens(tokens,length);
    freeArrayWithContents((void**)tokens,length);
    return newPlayerMeta(number,name);
}

char* getNameFromPlayerMetaTokens(char** tokens, size_t tokenCount){
    return joinTokens(&tokens[3],tokenCount - 3, " ");
}

char* getOtherPlayer(Connection* connection){
    ServerMessage* message = receiveServerMessage(connection);
    if (message->type == Error)
        panic(message->clearText);
    //TODO: Use Cleartext once imlemented
    char* out = copyStringToNewMemoryAddr(message->messageReference);
    freeServerMessage(message);
    return out;
}

int getTotalPlayers(Connection* connection){
    ServerMessage* message = receiveServerMessage(connection);
    if (message->type == Error)
        return -1;

    char ASCInum = message->messageReference[8];
    return ASCInum - '0';
}

int nextMessageIsEndplayers(Connection* connection){
    ServerMessage* message = receiveServerMessage(connection);
    if (message->type == Error)
        panic(message->clearText);
    int result = 0;
    if (message->type == Error)
        panic(message->messageReference);
    else if (strcmp(message->messageReference, ENDPLAYERS_COMMAND) == 0)
        result = 1;

    freeServerMessage(message);
    return result;
}

void sendClientVersion(Connection* connection, const char* version){
    char* message = concatStringToNewMemoryAddr(VERSION,version," ");
    logMessage(message);
    writeLineToServer(connection, message);
    free(message);
}

void sendGameId(Connection* connection, const char* gameID){
    char* message = concatStringToNewMemoryAddr(GAMEID,gameID," ");
    logMessage(message);
    writeLineToServer(connection, message);
    free(message);
}

void sendPlayerPreference(Connection* connection, const char* preference){
    if (preference == NULL){
        logMessage(PLAYERPREFERENCE);
        writeLineToServer(connection, PLAYERPREFERENCE);
    } else {
        char* message = concatStringToNewMemoryAddr(PLAYERPREFERENCE,preference," ");
        logMessage(message);
        writeLineToServer(connection, message);
        free(message);
    }
}

void logMessage(char* message){
    printf("Sending: '%s'\n",message);
}

ServerMessage* receiveServerMessage(Connection* connection) {
    return parseServerMessage(readLineFromServer(connection));
}

PlayerMeta* newPlayerMeta(int number, char* name){
    PlayerMeta* meta = malloc(sizeof(PlayerMeta));
    meta->name = copyStringToNewMemoryAddr(name);
    meta->number = number;
    return meta;
}

void freePlayerMeta(PlayerMeta* meta){
    free(meta->name);
    free(meta);
}