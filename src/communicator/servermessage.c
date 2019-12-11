#include <stdlib.h>
#include <stdio.h>
#include "servermessage_priv.h"
#include "misc/utilities.h"
#include <string.h>

#define MOVE_COMMAND "MOVE"
#define MOVEOK_COMMAND "MOVEOK"
#define WAIT_COMMAND "WAIT"
#define GAMEOVER_COMMAND "GAMEOVER"
#define ENDFIELD_COMMAND "ENDFIELD"
#define QUIT_COMMAND "QUIT"
#define OK_THINK_COMMAND "OKTHINK"
#define FIELD_COMMAND "FIELD"

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
    if(strcmp(splittedMessage[0], OK_THINK_COMMAND) == 0) 
        out = OkThink;
    if(strcmp(splittedMessage[0], FIELD_COMMAND) == 0) 
        out = FieldCommand;

    freeTokens(splittedMessage);
    return out;
}

int parseMoveTime(ServerMessage* message){
    if (message->type != Move)
        panic("message was not of type move!");

    size_t length = 0;
    char** tokens = slice(message->messageReference," ",&length);
    if (length < 3)
        panic("Unexpected token count!");
    
    int moveTime = atoi(tokens[2]);
    freeTokens(tokens);
    return moveTime;
}

int isError(const char* message){
    if (*message == '-')
        return 1;
    else 
        return 0;
}