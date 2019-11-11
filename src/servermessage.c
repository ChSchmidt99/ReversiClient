#include <stdlib.h>
#include <stdio.h>
#include "../include/servermessage.h"
#include "../include/utilities.h"


ServerMessage* newServerMessage(ServerMessageType type, ServerMessageCommand command, char* message, void* data){
    ServerMessage* serverMessage = malloc(sizeof(ServerMessage));
    serverMessage->type = type;
    serverMessage->command = command;
    serverMessage->message = copyStringToNewMemoryAddr(message);
    serverMessage->data = data;
    return serverMessage;
}

void freeServerMessage(ServerMessage* serverMessage){
    free(serverMessage->message);
    free(serverMessage->data);
    free(serverMessage);
}

ServerMessage* parseError(char* data){
    return newServerMessage(Error, Unknown, data, NULL);
}

ServerMessage* getServerVersionMessage(char* data){
    //TODO: Parse Version Number and add it to newServerMessage data
    char* message = "The Server is Accepting Connections!";
    return newServerMessage(Prolog, AcceptingConnections, message, NULL);
}

ServerMessage* parseServerVersion(char* data){
    if (*data == '+')
        return getServerVersionMessage(data += 2);
    else 
        return parseError(data += 2);
}

void printServerMessage(ServerMessage* serverMessage){
    printf("%s\n",serverMessage->message);
}