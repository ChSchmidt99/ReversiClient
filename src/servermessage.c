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