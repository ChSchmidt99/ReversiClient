#include <stdlib.h>
#include <stdio.h>
#include "communicator/servermessage.h"
#include "utilities.h"

ServerMessage* newServerMessage(ServerMessageType type, char* message, char* clearText, void* data){
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
    //TODO: Implement Me
    if (*message == '+') {
        message += 2;
        return newServerMessage(Prolog, message, message, NULL);
    } else {
        message += 2;
        return newServerMessage(Error,message, message, NULL);
    }
}

void printServerMessage(ServerMessage* serverMessage){
    //TODO: Implement Me
    printf("%s",serverMessage->clearText);
}