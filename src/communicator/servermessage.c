#include <stdlib.h>
#include <stdio.h>
#include "servermessage_priv.h"
#include "utilities.h"

ServerMessage* initServerMessage(char* messageReference, char* clearText, ServerMessageType type){
    ServerMessage* serverMessage = malloc(sizeof(ServerMessage));
    serverMessage->type = type;
    serverMessage->clearText = clearText;
    serverMessage->messageReference = messageReference;
    return serverMessage;
}

void freeServerMessage(ServerMessage* serverMessage){
    free(serverMessage->messageReference);
    free(serverMessage->clearText);
    free(serverMessage);
}

ServerMessage* parseServerMessage(char* message){
    ServerMessageType type = getType(message);
    char* clearText = getClearText(message);
    return initServerMessage(message, clearText, type);
}

/*
    vielleicht mit strcmp die "message" mit den möglichen Nachrichten vergleichen und demnach den Typ zurückgeben.
*/
ServerMessageType getType(const char* message){
    if(isError(message))
        return Error;

    return Prolog;
    //TODO: Implement Me
}

int isError(const char* message){
    if (*message == '-')
        return 1;
    else 
        return 0;
}

/*
    vielleicht eine bibliothek oder etwas ähnliches mit vordefinierten texten schreiben und ähnlich wie bei getType den 
    dementsprechenden Eintrag zurückgeben.
*/
char* getClearText(const char* message){
    //TODO: Implement Me!
    return "";
}