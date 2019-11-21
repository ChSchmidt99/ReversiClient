#include <stdlib.h>
#include <stdio.h>
#include "communicator/servermessage.h"
#include "utilities.h"

int unwrapError(char* message);
char* getClearText(char* message);
void* getData(char* message);

ServerMessage* newServerMessage(char* message, char* clearText, void* data, int isError){
    ServerMessage* serverMessage = malloc(sizeof(ServerMessage));
    serverMessage->isError = isError;
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
    int isError = unwrapError(message);
    char* clearText = getClearText(message);
    void* data = getData(message);
    return newServerMessage(message, clearText, data, isError);
}

int unwrapError(char* message){
    int isError = 0;
    
    if (*message == '-')
        isError = 1;
    
    message += 2;
    return isError;
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