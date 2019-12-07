#ifndef SERVERMESSAGE_H
#define SERVERMESSAGE_H

typedef enum _ServerMessageType {
    Error = 0,
    Prolog = 1,
    Wait = 2,
    Move = 3,
    Gameover = 4,
    MoveOk = 5
} ServerMessageType;

typedef struct _ServerMessage {
    char* clearText;
    char* message;
    void* data;
    ServerMessageType type;
} ServerMessage;

ServerMessage* newServerMessage(char* message, char* clearText, void* data, ServerMessageType type);
void freeServerMessage(ServerMessage* serverMessage);

//TODO: Implement Me
ServerMessage* parseServerMessage(char* message);
void printServerMessage(ServerMessage* serverMessage);

#endif