#ifndef SERVERMESSAGE_H
#define SERVERMESSAGE_H

typedef struct _ServerMessage {
    char* clearText;
    char* message;
    void* data;
    int isError;
} ServerMessage;

ServerMessage* newServerMessage(char* message, char* clearText, void* data, int isError);
void freeServerMessage(ServerMessage* serverMessage);

//TODO: Implement Me
ServerMessage* parseServerMessage(char* message);
void printServerMessage(ServerMessage* serverMessage);

#endif