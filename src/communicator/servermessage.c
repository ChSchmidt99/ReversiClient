#include <stdlib.h>
#include <stdio.h>
#include "servermessage_priv.h"
#include "utilities.h"

ServerMessage* newServerMessage(char* message, char* clearText, void* data, ServerMessageType type){
    ServerMessage* serverMessage = malloc(sizeof(ServerMessage));
    serverMessage->type = type;
    serverMessage->clearText = copyStringToNewMemoryAddr(clearText);
    serverMessage->message = copyStringToNewMemoryAddr(message);
    serverMessage->data = data;
    return serverMessage;
}

void freeServerMessage(ServerMessage* serverMessage){
    free(serverMessage->message);
    free(serverMessage->data);
    free(serverMessage->clearText);
    free(serverMessage);
}

ServerMessage* parseServerMessage(char* message){
    ServerMessageType type = getType(message);
    char* clearText = getClearText(message);
    void* data = getData(message);
    return newServerMessage(message, clearText, data, type);
}

ServerMessageType getType(char* message){
    if(isError(message))
        return Error;

    return Prolog;
    //TODO: Implement Me
}

int isError(char* message){
    if (*message == '-')
        return 1;
    else 
        return 0;
}

char* getClearText(char* message){
    //TODO: Implement Me!
    return message;
}

void* getData(char* message){
    //TODO: Implement Me!
    return message;
}

void printServerMessage(ServerMessage* serverMessage){
    //TODO: Implement Me!
    printf("%s",serverMessage->clearText);
}