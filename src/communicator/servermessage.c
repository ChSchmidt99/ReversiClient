#include <stdlib.h>
#include <stdio.h>
#include "servermessage_priv.h"
#include "utilities.h"
#include <string.h>

#define MOVE_COMMAND "MOVE"
#define MOVEOK_COMMAND "MOVEOK"
#define WAIT_COMMAND "WAIT"
#define GAMEOVER_COMMAND "GAMEOVER"


ServerMessage* initServerMessage(char* messageReference, ServerMessageType type){
    ServerMessage* serverMessage = malloc(sizeof(ServerMessage));
    serverMessage->type = type;
    serverMessage->messageReference = messageReference;
    return serverMessage;
}

void freeServerMessage(ServerMessage* serverMessage){
    free(serverMessage->messageReference);
    free(serverMessage);
}

ServerMessage* parseServerMessage(char* message){
    ServerMessageType type = getType(message);
    return initServerMessage(message, type);
}

ServerMessageType getType(const char* message){
    if (isError(message)) 
        return Error;
    
    size_t length = 0;
    char** splittedMessage = slice(message," ",&length);
    char* typeString = splittedMessage[1];
    ServerMessageType out = Prolog;
    if(strcmp(typeString, WAIT_COMMAND) == 0) 
        out = Wait;
    if(strcmp(typeString, GAMEOVER_COMMAND)) 
        out = Gameover;
    if(strcmp(typeString, MOVE_COMMAND)) 
        out = Move;
    if(strcmp(typeString, MOVEOK_COMMAND)) 
        out = MoveOk;

    freeTokens(splittedMessage);
    return out;
}

int isError(const char* message){
    if (*message == '-')
        return 1;
    else 
        return 0;
}