#include <stdlib.h>
#include <stdio.h>
#include "servermessage_priv.h"
#include "utilities.h"
#include <string.h>

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

char** splitMessage(char* message) {
    // reserve memory
    int sizeOfArray = 3;
    int maxSizeOfSubString = 100;
    char ** messageArray = malloc(sizeOfArray * sizeof(char*));
    for (int i = 0 ; i < sizeOfArray; ++i)
        messageArray[i] = malloc(maxSizeOfSubString * sizeof(char));
    // split the message in substrings, for exampe '+' 'WAIT' 'maximale Zugzeit'
    char ch;
    int spaceCounter = 0;
    char subString[100];
    char *pointer = subString;
    int pointerCounter = 0;
    //fill first substring ( + or -)
    pointer[0] = message[0];
    strcpy(messageArray[0], subString);
    memset(subString,0,sizeof(subString));
    //fill second and third substring
    for (int i = 2 ; i < strlen(message); ++i) {
        ch = message[i];
        if (ch == ' ') {
        	spaceCounter++;
        }
        pointer[pointerCounter++] = ch;
        if (spaceCounter == 1){
        		strcpy(messageArray[1], subString);
        		memset(subString,0,sizeof(subString));
        		pointerCounter = 0;
        		spaceCounter++;
        	}
    }
    strcpy(messageArray[2], subString);
    return messageArray;
}

ServerMessageType getType(const char* message){
    if (message[0]=='-') return Error;
    else {
        char** splittedMessage = splitMessage(message);
        char type[] = splittedMessage[1];
        if(type == "WAIT") return Wait;
        if(type == "MOVE") return Move;
        if(type == "GAMEOVER") return Gameover;
        if(type == "MOVEOK") return MoveOk;
        
    }
}

char* getClearText(const char* message){
    char** splittedMessage = splitMessage(message);
    return splittedMessage[3];
}