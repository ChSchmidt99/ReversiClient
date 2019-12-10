#include <stdlib.h>
#include <stdio.h>
#include "servermessage_priv.h"
#include "utilities.h"
#include <string.h>

#define MOVE_COMMAND "MOVE"
#define MOVEOK_COMMAND "MOVEOK"
#define WAIT_COMMAND "WAIT"
#define GAMEOVER_COMMAND "GAMEOVER"
#define ENDFIELD_COMMAND "ENDFIELD"
#define QUIT_COMMAND "QUIT"

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
    char** splittedMessage = slice(message + 2," ",&length);
    ServerMessageType out = Prolog;
    if(strcmp(splittedMessage[0], WAIT_COMMAND) == 0) 
        out = Wait;
    if(strcmp(splittedMessage[0], GAMEOVER_COMMAND) == 0) 
        out = Gameover;
    if(strcmp(splittedMessage[0], MOVE_COMMAND) == 0) 
        out = Move;
    if(strcmp(splittedMessage[0], MOVEOK_COMMAND) == 0) 
        out = MoveOk;
    if(strcmp(splittedMessage[0], ENDFIELD_COMMAND) == 0) 
        out = Endfield;
    if(strcmp(splittedMessage[0], QUIT_COMMAND) == 0) 
        out = Quit;

    freeTokens(splittedMessage);
    return out;
}

int isError(const char* message){
    if (*message == '-')
        return 1;
    else 
        return 0;
}