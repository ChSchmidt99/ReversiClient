#ifndef SERVERMESSAGE_H
#define SERVERMESSAGE_H

typedef enum _ServerMessageStage {
    Error = 0,
    Prolog = 1,
    Spielverlauf = 2,
    Spielzug = 3
} ServerMessageType;


typedef struct _ServerMessage {
    ServerMessageType type;
    char* clearText;
    char* message;
    void* data;
} ServerMessage;

ServerMessage* newServerMessage(ServerMessageType type, char* message, char* clearText, void* data);
void freeServerMessage(ServerMessage* serverMessage);

//TODO: Implement Me
ServerMessage* parseServerMessage(char* message);
void printServerMessage(ServerMessage* serverMessage);

#endif